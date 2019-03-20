#include "Model.h"

//===Node===
//constructors & destructor
Node::Node()
{
	//TODO
}

Node::Node(std::map<const aiNode*, UInt>& mapping, const aiNode* node)
{
    //TODO
    /*for (UInt i = 0; i < node->mNumChildren; i++)
    {
    }

    for (UInt i = 0; i < node->mNumMeshes; i++)
    {
        mMeshes[i] = node->mMeshes[i];
    }*/
}

Node::Node(Node&& node)
{
	//TODO
}


Node::~Node()
{}


//operators
Node& Node::operator = (Node&& node)
{
    if (this != &node)
    {
        //TODO
    }

    return *this;
}


//IObjectBase
const String& Node::toString() const
{
    return mName;
}


//get & set
const Node::UInt& Node::numChildren() const
{
    return mNumChildren;
}

const Node::Indices& Node::children() const
{
    return mChildren;
}


const Node::UInt& Node::numMeshes() const
{
    return mNumMeshes;
}

const Node::Indices& Node::meshes() const
{
    return mMeshes;
}


Mat4& Node::transform()
{
    return mTransform;
}

const Mat4& Node::transform() const
{
    return mTransform;
}
