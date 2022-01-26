#include <iostream>
#include <string>
#include <memory>
#include <sys/stat.h>

#include <ros/ros.h>
#include <ros/package.h>
#include <ros/publisher.h>
#include <ros/subscriber.h>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <glog/logging.h>

#include "planner.h"

using namespace std;
using namespace CICV_NS;

/// global vars
// map
nav_msgs::OccupancyGridConstPtr map_occ;
bool map_ok_flag = 0;

// start_state
State start_state;
bool start_ok = 0;
// end state
State end_state; // start and end state will be used to update the data member of Planner obj
bool end_ok = 0;

void cb_map(const nav_msgs::OccupancyGridConstPtr &msg)
{
    if (map_ok_flag)
        return;
    std::cout << "### received map message\n";
    cout << msg->data.size() << endl;
    map_occ = msg;
    map_ok_flag = 1;
}

void cb_start_state(const geometry_msgs::PoseWithCovarianceStampedConstPtr &msg)
{
    if (start_ok)
        return;
    start_state.x_ = msg->pose.pose.position.x;
    start_state.y_ = msg->pose.pose.position.y;
    // transfer the quanity to yaw angle
    // start_state.th_ = msg->pose.pose.orientation
    double x = msg->pose.pose.orientation.x;
    double y = msg->pose.pose.orientation.y;
    double z = msg->pose.pose.orientation.z;
    double w = msg->pose.pose.orientation.w;

    Eigen::Quaterniond q(w, x, y, z); //  w x y z
    Eigen::Vector3d euler_angle = q.matrix().eulerAngles(2, 1, 0);
    cout << euler_angle[0] << " " << euler_angle[1] << " " << euler_angle[2] << endl;
    start_ok = 1;
}

void cb_end_state(const geometry_msgs::PoseStampedConstPtr &msg)
{
    if (end_ok)
        return;

    end_state.x_ = msg->pose.position.x;
    end_state.y_ = msg->pose.position.y;
    // transfer the quanity to yaw angle
    // end_state.th_ = msg->pose.orientation
    double x = msg->pose.orientation.x;
    double y = msg->pose.orientation.y;
    double z = msg->pose.orientation.z;
    double w = msg->pose.orientation.w;

    Eigen::Quaterniond q(w, x, y, z); //  w x y z
    Eigen::Vector3d euler_angle = q.matrix().eulerAngles(2, 1, 0);
    cout << euler_angle[0] << " " << euler_angle[1] << " " << euler_angle[2] << endl;
    end_ok = 1;
}

int main(int argc, char **argv)
{
    std::string base_dir = ros::package::getPath("CICV-Contest");
    auto log_dir = base_dir + "/log";
    if (0 != access(log_dir.c_str(), 0))
    {
        // if this folder not exist, create a new one.
        mkdir(log_dir.c_str(), 0777);
    }

    google::InitGoogleLogging(argv[0]);
    FLAGS_colorlogtostderr = true;
    FLAGS_stderrthreshold = google::INFO;
    FLAGS_log_dir = log_dir;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 1000;
    FLAGS_stop_logging_if_full_disk = true;

    DLOG(INFO) << "debug mode";
    LOG(INFO) << "not debug mode";

    ros::init(argc, argv, "main_node");
    ros::NodeHandle nh;

    // TODO ： add log interface for this program
    // read map data from /map topic, read only one time
    ros::Subscriber suber_map = nh.subscribe("/map", 10, cb_map); //
    // 等待地图
    ros::Rate rate(10);
    // while (ros::ok() && map_ok_flag == 0)
    // {
    //     cout << "waiting for topic: \"/map\" \n";
    //     ros::spinOnce();
    //     rate.sleep();
    // }

    // 等待开始
    auto suber_start_state = nh.subscribe<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10, cb_start_state);
    while (ros::ok() && start_ok == 0)
    {
        cout << "waiting for topic: \"/initialpose\" \n";
        cout << start_ok << endl;
        ros::spinOnce();
        rate.sleep();
    }
    //结束pose
    auto suber_end_state = nh.subscribe<geometry_msgs::PoseStamped>("/move_base_simple/goal", 10, cb_end_state);
    while (ros::ok() && end_ok == 0)
    {
        cout << "waiting for topic: \"/move_base_simple/goal\" \n";
        cout << end_ok << endl;
        ros::spinOnce();
        rate.sleep();
    }

    // 开始规划
    std::shared_ptr<PlannerBase> planner(new PlannerHybridAStar());
    planner->start_state_ = start_state;
    planner->end_state_ = end_state;

    std::shared_ptr<VisualizationBase> vis(new VisualizationTraj()); // smart pointer: shared_ptr
    planner->visualizer = vis;

    while (ros::ok())
    {
        cout << "规划是否完成： " << planner->plan_ok_ << endl;
        if (planner->plan_ok_)
            break;
        planner->plan();
        planner->visualizer->show();
        rate.sleep();
    }

    return 0;
}
