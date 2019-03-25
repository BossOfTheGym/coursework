#pragma once

#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>
#include <Model/Model.h>


class Renderer
{
public:
    virtual void render(const Model& model);


    const ShaderProgram& program();

private:
    ShaderProgram mProgram;
};