#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <iostream>
#include <vector>

namespace CICV_NS
{
    class Vehicle
    {
    public:
        Vehicle() {}
        virtual ~Vehicle(){};

        double length_;
        double width_;
        double front_hang_;
        double rear_hang_;
        double wheel_base_;

        void kinematic_model_update();
    };
}

#endif
