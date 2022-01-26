#ifndef Planner_H_
#define Planner_H_

#include <iostream>
#include <memory>

#include <nav_msgs/OccupancyGrid.h>

#include "data_structure.h"
#include "visualization.h"

namespace CICV_NS
{
    // Abstract Planner
    class PlannerBase
    {
    private:
    public:
        PlannerBase(){};
        virtual ~PlannerBase(){};
        virtual bool plan() = 0;

        // VisualizationBase *visualizer; // 每个规划器都带有一个可视化基类指针
        std::shared_ptr<VisualizationBase> visualizer;

        bool flag_start_state_ok_ = 0;
        bool flag_end_state_ok = 0;

        State start_state_;
        State end_state_;

        bool plan_ok_ = 0;

        bool checkValidState(const State &); // check if the state is occupied in map
        bool checkValidPath(const Traj &);
    };

    class PlannerHybridAStar : public PlannerBase
    {
    public:
        PlannerHybridAStar(){};
        virtual ~PlannerHybridAStar(){};

        virtual bool plan() override;
    };
}

#endif
