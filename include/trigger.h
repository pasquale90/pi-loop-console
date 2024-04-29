#ifndef EVENT_H
#define EVENT_H

#include "controls.h"

struct Trigger{
    Control id;
    bool isHolded;
    bool isMsg;
};

#endif