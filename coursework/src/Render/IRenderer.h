#pragma once


#include <Model/Model.h>


class IRenderer
{
public:
    virtual void render(const Model& model) = 0;
};