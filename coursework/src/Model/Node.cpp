#include "Model.h"

//===Node===
//constructors & destructor
Node::Node()
	: mName("")
	, mNumChildren(0)
	, mNumMeshes(0)
	, mMeshes(nullptr)
	, mChildren(nullptr)
{}

Node::Node(const aiNode* node, const std::map<const aiNode*, UInt>& mapping) : Node()
{
	if (node)
	{
		mNumMeshes = node->mNumMeshes;
		mNumChildren = node->mNumChildren;
		mMeshes.reset(mNumMeshes ? new UInt[mNumMeshes]() : nullptr);
		mChildren.reset(mNumChildren ? new UInt[mNumChildren]() : nullptr);
		mName = node->mName.C_Str();

		for (UInt i = 0; i < node->mNumChildren; i++)
		{
			auto it = mapping.find(node->mChildren[i]);
			mChildren[i] = it->second;
		}

		for (UInt i = 0; i < node->mNumMeshes; i++)
		{
			mMeshes[i] = node->mMeshes[i];
		}
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
		std::swap(mNumChildren, node.mNumChildren);
		swap(mChildren, node.mChildren);

		std::swap(mNumMeshes, node.mNumMeshes);
		std::swap(mMeshes, node.mMeshes);
    }

    return *this;
}


//IObjectBase
const String& Node::toString() const
{
    return mName;
}


//get & set
const UInt& Node::numChildren() const
{
    return mNumChildren;
}

const UInt* Node::children() const
{
    return mChildren.get();
}


const UInt& Node::numMeshes() const
{
    return mNumMeshes;
}

const UInt* Node::meshes() const
{
    return mMeshes.get();
}
