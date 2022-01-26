#ifndef DATA_STRUCTURE_H_
#define DATA_STRUCTURE_H_

#include <vector>

#include <iostream>
using namespace std;

namespace CICV_NS
{
    struct State
    {
        double x_;
        double y_;
        double th_;
    };

    struct Node
    {
        State state_;
        Node *next_node_;
    };

    using Traj = std::vector<State>; // trajectory
}

#endif
