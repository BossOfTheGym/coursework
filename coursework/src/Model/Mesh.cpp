#include "Model.h"


//===ObjectBlock===


//===SharedBlock===
Mesh::SharedBlock::SharedBlock(Mesh::UInt materialIndex, Mesh::VAB&& vao, const String& name)
    : mMaterialIndex(materialIndex)
    , mVertexBuffer(std::move(vao))
{}


//===Mesh===
//constructors & destructors
Mesh::Mesh()
    : mObjectBlock()
    , mSharedBlock()
{}

Mesh::Mesh(const aiMesh* mesh, const String& name = "")
    : mObjectBlock()
    , mSharedBlock(new SharedBlock())
{
    //name
    mSharedBlock->mName = name;

    //fill
    std::vector<float> vertices;
    if (mesh->HasPositions())
    {
        for (UInt i = 0; i < mesh->mNumFaces; i++)
        {
            auto& face = mesh->mFaces[i];

            for (UInt j = 0; j < face.mNumIndices; j++)
            {
                auto& index  = face.mIndices[j];

                auto& vertex = mesh->mVertices[index];
                vertices.push_back(vertex[0]);
                vertices.push_back(vertex[1]);
                vertices.push_back(vertex[2]);
            }
        }
    }


    std::vector<float> colors;
    if (mesh->HasVertexColors(0))
    {

    }


    std::vector<float> normals;
    if (mesh->HasNormals())
    {
        for (UInt i = 0; i < mesh->mNumFaces; i++)
        {
            auto& face = mesh->mFaces[i];

            for (UInt j = 0; j < face.mNumIndices; j++)
            {
                auto& index  = face.mIndices[j];

                auto& normal = mesh->mNormals[index];
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
            }
        }
    }


    std::vector<float> tangents;
    std::vector<float> bitangents;
    if (mesh->HasTangentsAndBitangents())
    {
        for (UInt i = 0; i < mesh->mNumFaces; i++)
        {
            auto& face = mesh->mFaces[i];

            for (UInt j = 0; j < face.mNumIndices; j++)
            {
                auto& index = face.mIndices[j];

                auto& tangent = mesh->mTangents[index];
                tangents.push_back(tangent[0]);
                tangents.push_back(tangent[1]);
                tangents.push_back(tangent[2]);

                auto& bitangent = mesh->mBitangents[index];
                bitangents.push_back(bitangent[0]);
                bitangents.push_back(bitangent[1]);
                bitangents.push_back(bitangent[2]);
            }
        }
    }


    std::vector<float> textureCoords;
    if (mesh->HasTextureCoords(0))
    {

    }
    

    //materials
    mSharedBlock->mMaterialIndex = mesh->mMaterialIndex;
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


//IObjectBase
const String& Mesh::toString() const
{
    return mSharedBlock->mName;
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