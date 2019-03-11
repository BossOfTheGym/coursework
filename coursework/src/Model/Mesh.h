#pragma once


#include "../Common.h"
#include "../Texture/Texture2D.h"
#include "../Shader/ShaderProgram.h"


#include "VertexArrayBuffer.h"


class Mesh
{
public:
    using TexturePointer = std::shared_ptr<Texture2D>;
    using VertexArrayPointer = std::shared_ptr<VertexArrayBuffer>;


public:
    Mesh();


private:
    std::vector<TexturePointer> textures;
    
    VertexArrayPointer vertexBuffer;
};