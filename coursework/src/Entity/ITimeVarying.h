#pragma once

class ITimeVarying
{
public:
    void update(float t, float dt) = 0;
};