#include "Renderer.h"

const ShaderProgram& Renderer::program()
{
    return mProgram;
}

const std::vector<Texture2D>& Renderer::textures()
{
    return mTextures;
}