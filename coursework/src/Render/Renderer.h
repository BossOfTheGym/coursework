#pragma once


#include "../Texture/Texture2D.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderProgram.h"
#include "../Model/Model.h"


class Renderer
{
public:
    virtual void render(const Model& model);


    const ShaderProgram& program();

    const std::vector<Texture2D>& textures();


private:
    std::vector<Texture2D> mTextures;
    ShaderProgram mProgram;
};