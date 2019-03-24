#include "Model.h"

//statics
namespace
{
	Mat4 toMat4(const aiMatrix4x4& mat)
	{
		return Mat4(
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3]
		);
	}

	auto loadNodes(const aiScene* scene)
	{
		Model::Nodes nodes;
		Model::Transformations nodesTransformations;


		//count nodes
		std::function<UInt(const aiNode*)> countNodes = 
			[&] (const aiNode* node) -> UInt
		{
			UInt count = 0;

			if (node != nullptr)
			{
				//count self
				count = 1;
				for (UInt i = 0; i < node->mNumChildren; i++)
				{
					count += countNodes(node->mChildren[i]);
				}
			}

			return count;
		};

		UInt count = countNodes(scene->mRootNode);
		nodes.resize(count);
		nodesTransformations.resize(count);


		//fill nodes data
		std::function<void(UInt&, std::map<const aiNode*, UInt>&, const aiNode*)> fillNodes
			= [&] (UInt& label, std::map<const aiNode*, UInt>& mapping, const aiNode* node) -> void
		{
			if (node != nullptr)
			{
				UInt currLabel = label;

				mapping[node] = label++;
				for (UInt i = 0; i < node->mNumChildren; i++)
				{
					fillNodes(label, mapping, node->mChildren[i]);
				}

				nodes[currLabel] = std::move(Node::buildFromAssimp(node, mapping));
				nodesTransformations[currLabel] = toMat4(node->mTransformation);
			}
		};

		UInt label = 0;
		std::map<const aiNode*, UInt> mapping;
		fillNodes(label, mapping, scene->mRootNode);


		return std::make_tuple(std::move(nodes), std::move(nodesTransformations));
	}

	auto loadMeshes(const aiScene* scene)
	{
		Model::Meshes meshes;

		if (scene->HasMeshes())
		{
			meshes.resize(scene->mNumMeshes);
			for (UInt i = 0; i < meshes.size(); i++)
			{
				meshes[i] = std::move(Mesh::buildFromAssimp(scene->mMeshes[i]));
			}
		}

		return meshes;
	}

	auto loadMaterials(const aiScene* scene)
	{
		Model::Materials materials;

		if (scene->HasMaterials())
		{
			materials.resize(scene->mNumMaterials);
			for (UInt i = 0; i < scene->mNumMaterials; i++)
			{
				materials[i] = std::move(Material::buildFromAssimp(scene->mMaterials[i], std::to_string(i)));
			}
		}	

		return materials;
	}
}

Model Model::buildFromAssimp(const String& location, const String& name)
{
	Model result;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		location,
		aiProcess_CalcTangentSpace 
		| aiProcess_Triangulate 
		| aiProcess_JoinIdenticalVertices 
		| aiProcess_SortByPType 
		| aiProcess_OptimizeMeshes 
	);

	if (scene)
	{
		//name
		result.mName = name;

		//nodes
		auto[nodes, nodesTransformations] = std::move(loadNodes(scene));
		result.mNodes = std::move(nodes);
		result.mNodesTransformations = std::move(nodesTransformations);

		//meshes
		result.mMeshes = std::move(loadMeshes(scene));

		//materials
		result.mMaterials = std::move(loadMaterials(scene));
	}
	else
	{
		std::cerr << "Failed to load model: " << location << std::endl;
	}
	
	return result;
}

Model Model::buildFromData()
{
	return Model();
}


//constructors & destructor
Model::Model()
	: mMeshes()
	, mNodes()
	, mNodesTransformations()
	, mMaterials()
	, mName("")
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
		
		std::swap(mName, model.mName);
    }

    return *this;
}


//IObject
const String& Model::toString() const
{
    return mName;
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