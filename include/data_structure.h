#ifndef DATA_STRUCTURE_H_
#define DATA_STRUCTURE_H_

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
}

#endif
