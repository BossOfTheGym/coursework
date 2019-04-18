#pragma once

#include "Time.h"

class ITimeVarying
{
public:
    virtual void update(const Time& t, const Time& dt) = 0;
};