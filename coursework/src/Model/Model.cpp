#include "Model.h"


//===ObjectBlock


//===SharedBlock===
Model::SharedBlock::SharedBlock(UInt numMeshes, UInt numNodes, UInt numMaterials, const String& name)
    : mNumMeshes(numMeshes)
    , mNumNodes(numNodes)
    , mNumMaterials(numMaterials)
    , mMeshes(new Mesh[numMeshes])
    , mNodes(new Node[numNodes])
    , mMaterials(new Texture2D[numMaterials])//TODO
    , mName(name)
{}


//===Model===
//constructors & destructor
Model::Model()
    : mObjectBlock()
    , mSharedBlock()
{}


Model::Model(const aiScene* scene, const String& name)
    : mObjectBlock()
    , mSharedBlock(new SharedBlock())
{
    std::function<UInt(const aiNode*)> countNodes 
        = [&] (const aiNode* node) -> UInt
    {
        UInt count = 0;

        if (node != nullptr)
        {
            count = node->mNumChildren;
            for (UInt i = 0; i < node->mNumChildren; i++)
            {
                count += countNodes(node->mChildren[i]);
            }
        }

        return count;
    };

    std::function<void(UInt&, std::map<const aiNode*, UInt>&, const aiNode*)> fillNodes 
        = [&, this] (UInt& label, std::map<const aiNode*, UInt>& mapping, const aiNode* node) -> void
    {
        if (node != nullptr)
        {
            UInt currLabel = label;

            mapping[node] = label++;
            for (UInt i = 0; i < node->mNumChildren; i++)
            {
                fillNodes(label, mapping, node->mChildren[i]);
            }

            mSharedBlock->mNodes[currLabel] = Node(mapping, node);
        }
    };

    //name
    mSharedBlock->mName = name;

    //nodes
    UInt nodes = countNodes(scene->mRootNode);
    mSharedBlock->mNumNodes = nodes;
    mSharedBlock->mNodes.reset(new Node[nodes]);

    UInt label = 0;
    std::map<const aiNode*, UInt> mapping;
    fillNodes(label, mapping, scene->mRootNode);

    //meshes
    UInt meshes = scene->mNumMeshes;
    mSharedBlock->mNumMeshes = meshes;
    mSharedBlock->mMeshes.reset(new Mesh[meshes]);
    for (UInt i = 0; i < meshes; i++)
    {
        mSharedBlock->mMeshes[i] = Mesh(scene->mMeshes[i]);
    }

    //materials
    
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


//IObjectBase
const String& Model::toString() const
{
    return mSharedBlock->mName;
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
