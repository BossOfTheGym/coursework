#include "Node.h"


//constructors & destructor
Node::Node()
	: mName("")
	, mMeshes()
	, mChildren()
{}

Node::Node(Indices&& children, Indices&& meshes, String&& name)
	: mChildren(std::move(children))
	, mMeshes(std::move(meshes))
	, mName(std::move(name))
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


const String& Node::name() const
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
