#include "PlanetModel.h"


//statics
const UInt PlanetNode::MESH = 0;

const UInt PlanetNode::NUM_MESHES = 1;

const UInt PlanetNode::NUM_CHILDREN = 0;


//constructors & destructor
PlanetNode::PlanetNode()
	: mName("")
{}

PlanetNode::PlanetNode(const String& name)
	: mName(name)
{}

PlanetNode::PlanetNode(PlanetNode&& node) : PlanetNode()
{
	*this = std::move(node);
}


PlanetNode::~PlanetNode()
{}


//operators
PlanetNode& PlanetNode::operator = (PlanetNode&& node)
{
	if (this != &node)
	{
		std::swap(mName, node.mName);
	}

	return *this;
}


//IObject
const String& PlanetNode::toString() const
{
	return mName;
}


//INode
const UInt& PlanetNode::numChildren() const
{
	return NUM_CHILDREN;
}

const UInt* PlanetNode::children() const
{
	return nullptr;
}


const UInt& PlanetNode::numMeshes() const
{
	return NUM_MESHES;
}

const UInt* PlanetNode::meshes() const
{
	return &MESH;
}