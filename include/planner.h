#ifndef Planner_H_
#define Planner_H_

namespace CICV_NS
{

    class VisualizationBase
    {
    private:
    public:
        VisualizationBase(){};
        virtual ~VisualizationBase(){};
        virtual bool show() = 0; // show on rviz
    };

    // Abstract Planner
    class PlannerBase
    {
    private:
    public:
        PlannerBase(){};
        virtual ~PlannerBase(){};
        virtual bool plan() = 0;

        VisualizationBase *visualizer; // 每个规划器都带有一个可视化基类指针
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
