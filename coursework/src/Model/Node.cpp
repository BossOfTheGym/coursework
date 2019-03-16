#include "Model.h"


//===ObjectBlock===
//constructors and destructors
Node::ObjectBlock::ObjectBlock(Node::UInt parent, const aiMatrix4x4& transform)
    : mParent(parent)
    , mTransform(aiMatrix4x4
          transform[0][0], transform[1][0], transform[2][0], transform[3][0]
        , transform[0][1], transform[1][1], transform[2][1], transform[3][1]
        , transform[0][2], transform[1][2], transform[2][2], transform[3][2]
        , transform[0][3], transform[1][3], transform[2][3], transform[3][3]
    )
{}


//===SharedBlock===
//constructor and destructor
Node::SharedBlock::SharedBlock(Node::UInt numChildren, Node::UInt numMeshes)
    : mNumChildren(numChildren)
    , mNumMeshes(numMeshes)
    , mChildren(new Node::UInt[numChildren])
    , mMeshes(new Node::UInt[numMeshes])
{}


//===Node===
//constructors & destructor
Node::Node(const aiNode* node)
{
    //TODO
}


Node::Node(const Node& node)
{
    mObjectBlock = node.mObjectBlock;
    mSharedBlock = node.mSharedBlock;
}


Node::Node(Node&& node)
{
    mObjectBlock = node.mObjectBlock;
    mSharedBlock = node.mSharedBlock;
}


Node::~Node()
{}


//operators
Node& Node::operator = (const Node& node)
{
    if (this != &node)
    {
        mObjectBlock = node.mObjectBlock;
        mSharedBlock = node.mSharedBlock;
    }

    return *this;
}


Node& Node::operator = (Node&& node)
{
    if (this != &node)
    {
        mObjectBlock = node.mObjectBlock;
        mSharedBlock = node.mSharedBlock;
    }

    return *this;
}


//get & set
const Node::UInt& Node::numChildren() const
{
    return mSharedBlock->mNumChildren;
}

const Node::Indices& Node::children() const
{
    return mSharedBlock->mChildren;
}


const Node::UInt& Node::numMeshes() const
{
    return mSharedBlock->mNumMeshes;
}

const Node::Indices& Node::meshes() const
{
    return mSharedBlock->mMeshes;
}


Mat4& Node::transform()
{
    return mObjectBlock.mTransform;
}

const Mat4& Node::transform() const
{
    return mObjectBlock.mTransform;
}


const Node::UInt& Node::parent() const
{
    return mObjectBlock.mParent;
}