#include "AssimpModel.h"


//constructors & destructor
AssimpNode::AssimpNode()
	: mName("")
	, mNumChildren(0)
	, mNumMeshes(0)
	, mMeshes(nullptr)
	, mChildren(nullptr)
{}

AssimpNode::AssimpNode(const aiNode* node, const std::map<const aiNode*, UInt>& mapping) : AssimpNode()
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

AssimpNode::AssimpNode(AssimpNode&& node)
{
	*this = std::move(node);
}


AssimpNode::~AssimpNode()
{}


//operators
AssimpNode& AssimpNode::operator = (AssimpNode&& node)
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


//IObject
const String& AssimpNode::toString() const
{
    return mName;
}


//INode
const UInt& AssimpNode::numChildren() const
{
    return mNumChildren;
}

const UInt* AssimpNode::children() const
{
    return mChildren.get();
}


const UInt& AssimpNode::numMeshes() const
{
    return mNumMeshes;
}

const UInt* AssimpNode::meshes() const
{
    return mMeshes.get();
}
