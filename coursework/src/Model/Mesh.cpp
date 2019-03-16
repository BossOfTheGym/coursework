#include "Model.h"


//===ObjectBlock===


//===SharedBlock===
Mesh::SharedBlock::SharedBlock(Mesh::UInt materialIndex, Mesh::VAB&& vao)
    : mMaterialIndex(materialIndex)
    , mVertexBuffer(std::move(vao))
{}


//===Mesh===
//constructors & destructors
Mesh::Mesh(const aiMesh* mesh)
{
    //TODO
}


Mesh::Mesh(const Mesh& mesh)
{
    mObjectBlock = mesh.mObjectBlock;
    mSharedBlock = mesh.mSharedBlock;
}


Mesh::Mesh(Mesh&& mesh)
{
    mObjectBlock = mesh.mObjectBlock;
    mSharedBlock = mesh.mSharedBlock;
}


Mesh::~Mesh()
{}


//operators
Mesh& Mesh::operator = (const Mesh& mesh)
{
    if (this != &mesh)
    {
        mObjectBlock = mesh.mObjectBlock;
        mSharedBlock = mesh.mSharedBlock;
    }

    return *this;
}


Mesh& Mesh::operator = (Mesh&& mesh)
{
    if (this != &mesh)
    {
        mObjectBlock = mesh.mObjectBlock;
        mSharedBlock = mesh.mSharedBlock;
    }

    return *this;
}


//get & set
const Mesh::VAB& Mesh::vab() const
{
    return mSharedBlock->mVertexBuffer;
}

const Mesh::UInt& Mesh::material() const
{
    return mSharedBlock->mMaterialIndex;
}