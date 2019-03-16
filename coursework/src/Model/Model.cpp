#include "Model.h"


//===ObjectBlock


//===SharedBlock===
Model::SharedBlock::SharedBlock(UInt numMeshes, UInt numNodes)
    : mNumMeshes(numMeshes)
    , mNumNodes(numNodes)
    , mMeshes(new Mesh[numMeshes])
    , mNodes(new Node[numNodes])
{}


//===Model===
//constructors & destructor
Model::Model()
    : mObjectBlock()
    , mSharedBlock()
{}


Model::Model(const aiScene* scene)
{
    //TODO
}


Model::Model(const Model& model)
{
    mObjectBlock = model.mObjectBlock;
    mSharedBlock = model.mSharedBlock;
}


Model::Model(Model&& model)
{
    mObjectBlock = model.mObjectBlock;
    mSharedBlock = model.mSharedBlock;
}


Model::~Model()
{}


//operators
Model& Model::operator = (const Model& model)
{
    if (this != &model)
    {
        mObjectBlock = model.mObjectBlock;
        mSharedBlock = model.mSharedBlock;
    }

    return *this;
}

Model& Model::operator = (Model&& model)
{
    if (this != &model)
    {
        mObjectBlock = model.mObjectBlock;
        mSharedBlock = model.mSharedBlock;
    }

    return *this;
}


//get & set
const Model::UInt& Model::numMeshes() const
{
    return mSharedBlock->mNumMeshes;
}

const Model::Meshes& Model::meshes() const
{
    return mSharedBlock->mMeshes;
}


const Model::UInt& Model::numNodes() const
{
    return mSharedBlock->mNumNodes;
}

const Model::Nodes& Model::nodes() const
{
    return mSharedBlock->mNodes;
}


const Node& Model::root() const
{
    return mSharedBlock->mNodes[0];
}
