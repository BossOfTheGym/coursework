#pragma once


#include "../Common.h"
#include "../Texture/Texture2D.h"


#include "VertexArrayBuffer.h"


class Mesh
{
public:

private:
    std::vector<Texture2D> textures;
    
    VertexArrayBuffer vertexBuffer;
};