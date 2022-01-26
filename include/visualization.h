#ifndef VISUALIZATION_H_
#define VISUALIZATION_H_

#include <iostream>
using namespace std;
namespace CICV_NS
{
    // Visualization
    class VisualizationBase
    {
    public:
        VisualizationBase(){};
        virtual ~VisualizationBase(){};
        virtual bool show() = 0; // show on rviz
    };

    class VisualizationTraj : public VisualizationBase
    {
    public:
        VisualizationTraj() {}
        virtual ~VisualizationTraj(){};
        bool show() override;
    };
}

#endif
