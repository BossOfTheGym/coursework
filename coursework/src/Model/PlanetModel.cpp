#include "PlanetModel.h"


//statics
const UInt PlanetModel::NUM_MESHES = 1;

const UInt PlanetModel::NUM_NODES = 1;

const UInt PlanetModel::NUM_MATERIALS = 1;


//constructors & destructor
PlanetModel::PlanetModel()
	: mNode()
	, mNodePtr(&mNode)
	, mMesh()
	, mMeshPtr(&mMesh)
	, mMaterial()
	, mMaterialPtr(&mMaterial)
	, mTransformation()
	, mTransformationPtr(&mTransformation)
	, mName("")
{}

PlanetModel::PlanetModel(
	UInt split
	, const String& diffuseLocation
	, const String& name
)
	: PlanetModel()
{
	mName = name;

	mNode = std::move(PlanetNode(name));
	mMesh = std::move(PlanetMesh(split, name));
	mMaterial = std::move(PlanetMaterial(diffuseLocation, name));
	mTransformation = std::move(Mat4(1.0f));
}

PlanetModel::PlanetModel(PlanetModel&& model)
	: PlanetModel()
{
	*this = std::move(model);
}


PlanetModel::~PlanetModel()
{}


//operators
PlanetModel& PlanetModel::operator = (PlanetModel&& model)
{
	if (this != &model)
	{
		std::swap(mNode, model.mNode);
		std::swap(mMesh, model.mMesh);
		std::swap(mMaterial, model.mMaterial);
		std::swap(mTransformation, model.mTransformation);
		std::swap(mName, model.mName);
	}

	return *this;
}


//IObject
const String& PlanetModel::toString() const
{
	return mName;
}


//IModel
const UInt& PlanetModel::numMeshes() const
{
	return NUM_MESHES;
}

const IMesh** PlanetModel::meshes() const
{
	return (const IMesh**)&mMeshPtr;
}


const UInt& PlanetModel::numNodes() const
{
	return NUM_NODES;
}

const INode** PlanetModel::nodes() const
{
	return (const INode**)&mNodePtr;
}

const Mat4** PlanetModel::transformations() const
{
	return (const Mat4**)&mTransformationPtr;
}


const UInt& PlanetModel::numMaterials() const
{
	return NUM_MATERIALS;
}

const IMaterial** PlanetModel::materials() const
{
	return (const IMaterial**)&mMaterialPtr;
}