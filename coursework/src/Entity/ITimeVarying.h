#pragma once

class ITimeVarying
{
public:
    virtual void update(float t, float dt) = 0;
};