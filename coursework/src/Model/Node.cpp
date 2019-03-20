#include "Model.h"

//===Node===
//constructors & destructor
Node::Node()
	: mNumMeshes(0)
	, mNumChildren(0)
	, mMeshes(nullptr)
	, mChildren(nullptr)
	, mName("")
{}

Node::Node(std::map<const aiNode*, UInt>& mapping, const aiNode* node)
	: mNumMeshes(node->mNumMeshes)
	, mNumChildren(node->mNumChildren)
	, mMeshes(new UInt[mNumMeshes])
	, mChildren(new UInt[mNumChildren])
	, mName(node->mName.C_Str())
{
    for (UInt i = 0; i < node->mNumChildren; i++)
    {
		mChildren[i] = mapping[node->mChildren[i]];
    }

    for (UInt i = 0; i < node->mNumMeshes; i++)
    {
        mMeshes[i] = node->mMeshes[i];
    }
}

Node::Node(Node&& node)
{
	*this = std::move(node);
}


Node::~Node()
{}


//operators
Node& Node::operator = (Node&& node)
{
    if (this != &node)
    {
		mNumChildren = node.mNumChildren;
		mChildren.swap(node.mChildren);

		mNumMeshes = node.mNumMeshes;
		mMeshes.swap(node.mMeshes);
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
