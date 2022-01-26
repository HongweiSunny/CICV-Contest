#include <iostream>
#include <nav_msgs/OccupancyGrid.h>
#include <ros/ros.h>

#include <ros/publisher.h>
#include <ros/subscriber.h>

#include "planner.h"

using namespace std;
using namespace CICV_NS;

void cb_map(const nav_msgs::OccupancyGridConstPtr msg)
{
    // how to put the msg data into a variable
}

int main()
{
    ros::NodeHandle nh;
    // TODO ： add log interface for this program
    // read map data from /map topic, read only one time
    ros::Subscriber suber_map = nh.subscribe("/map", 10, ); //

    PlannerHybridAStar plannerhyas;

    cout << "hello world\n";
    return 0;
}
