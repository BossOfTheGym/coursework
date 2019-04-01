#include "Model.h"


//constructors & destructor
Model::Model()
	: mMeshes()
	, mNodes()
	, mNodesTransformations()
	, mMaterials()
{}

Model::Model(
	Meshes&& meshes
	, Nodes&& nodes
	, Transformations&& nodesTransformations
	, Materials&& materials
)
	: mMeshes(std::move(meshes))
	, mNodes(std::move(nodes))
	, mNodesTransformations(std::move(nodesTransformations))
	, mMaterials(std::move(materials))
{}

Model::Model(Model&& model)
{
	*this = std::move(model);
}


Model::~Model()
{}


//operators
Model& Model::operator = (Model&& model)
{
    if (this != &model)
    {
		std::swap(mMeshes, model.mMeshes);
		std::swap(mNodes, model.mNodes);
		std::swap(mNodesTransformations, model.mNodesTransformations);		
		std::swap(mMaterials, model.mMaterials);
    }

    return *this;
}


//Model
const Model::Meshes& Model::meshes() const
{
    return mMeshes;
}


const Model::Nodes& Model::nodes() const
{
    return mNodes;
}

const Model::Transformations& Model::transformations() const
{
	return mNodesTransformations;
}


const Model::Materials& Model::materials() const
{
	return mMaterials;
}