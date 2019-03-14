#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "../Common.h"
#include "../Texture/Texture2D.h"
#include "../Shader/ShaderProgram.h"


#include "Model.h"
#include "VertexArrayBuffer.h"

class Model;

class Mesh
{
public:
    using TexturePointer = std::shared_ptr<Texture2D>;
    using VertexArrayPointer = std::shared_ptr<VertexArrayBuffer>;

public:
    Mesh(Model& model, const aiMesh* mesh);

    Mesh(const Mesh& mesh);

    Mesh(Mesh&& mesh);


    ~Mesh();


    Mesh& operator = (const Mesh& mesh) = delete;

    Mesh& operator = (Mesh&& mesh);


    const VertexArrayBuffer& vab() const;


    const int material() const;

    int material();


private:
    //parent
    //non-assignable
    Model* mModel;//????

    //shared
    VertexArrayPointer mVertexBuffer;
    
    //shared
    int mMaterialIndex;
};