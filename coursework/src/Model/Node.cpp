#include "Model.h"

//statics
Node Node::buildFromAssimp(const aiNode* node, const std::map<const aiNode*, UInt>& mapping)
{
	Node result;

	if (node)
	{
		//name
		result.mName = node->mName.C_Str();

		//node children
		result.mChildren.resize(node->mNumChildren);
		for (UInt i = 0; i < node->mNumChildren; i++)
		{
			auto it = mapping.find(node->mChildren[i]);
			result.mChildren[i] = it->second;
		}

		//node meshes
		result.mMeshes.resize(node->mNumMeshes);
		for (UInt i = 0; i < node->mNumMeshes; i++)
		{
			result.mMeshes[i] = node->mMeshes[i];
		}
	}

	return result;
}

Node Node::buildFromData()
{
	return Node();
}


//constructors & destructor
Node::Node()
	: mName("")
	, mMeshes()
	, mChildren()
{}

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
		std::swap(mChildren, node.mChildren);
		std::swap(mMeshes, node.mMeshes);

		std::swap(mName, node.mName);
    }

    return *this;
}


//IObject
const String& Node::toString() const
{
    return mName;
}


//Node
const Node::Indices& Node::children() const
{
    return mChildren;
}

const Node::Indices& Node::meshes() const
{
    return mMeshes;
}
