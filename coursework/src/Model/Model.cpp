#include "Model.h"


//===Model===
//constructors & destructor
Model::Model()
	: mNumMeshes(0)
	, mMeshes(nullptr)
	, mNumNodes(0)
	, mNodes(nullptr)
	, mNodeTransformations(nullptr)
	, mNumMaterials(0)
	, mMaterials(nullptr)
	, mName("")
{}

Model::Model(const aiScene* scene, const String& name) : Model()
{
	if (scene)
	{
		loadModel(scene, name);
	}
}

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
		std::swap(mNumMeshes, model.mNumMeshes);
		std::swap(mMeshes, model.mMeshes);

		std::swap(mNumNodes, model.mNumNodes);
		std::swap(mNodes, model.mNodes);
		std::swap(mNodeTransformations, model.mNodeTransformations);

		std::swap(mNumMaterials, model.mNumMaterials);
		std::swap(mMaterials, model.mMaterials);
    }

    return *this;
}


//IObjectBase
const String& Model::toString() const
{
    return mName;
}


//get & set
const UInt& Model::numMeshes() const
{
    return mNumMeshes;
}

const IMesh* Model::meshes() const
{
    return mMeshes.get();
}


const UInt& Model::numNodes() const
{
    return mNumNodes;
}

const INode* Model::nodes() const
{
    return mNodes.get();
}

const Mat4* Model::transformations() const
{
	return mNodeTransformations.get();
}


const INode& Model::root() const
{
    return mNodes[0];
}


//load
void Model::loadModel(const aiScene* scene, const String& name)
{
	//name
	mName = name;

	//nodes
	UInt nodes = countNodes(scene->mRootNode);
	mNumNodes = nodes;
	mNodes.reset(mNumNodes ? new Node[mNumNodes]() : nullptr);
	mNodeTransformations.reset(mNumNodes ? new Mat4[mNumNodes]() : nullptr);

	UInt label = 0;
	std::map<const aiNode*, UInt> mapping;
	fillNodes(label, mapping, scene->mRootNode);


	//meshess
	if (scene->HasMeshes())
	{
		UInt meshes = scene->mNumMeshes;
		mNumMeshes = meshes;
		mMeshes.reset(mNumMeshes ? new Mesh[meshes]() : nullptr);
		for (UInt i = 0; i < meshes; i++)
		{
			mMeshes[i] = Mesh(scene->mMeshes[i]);
		}
	}

	//materials
	if (scene->HasMaterials())
	{
		mNumMaterials = scene->mNumMaterials;
		mMaterials.reset(mNumMaterials ? new Material[mNumMaterials]() : nullptr);
		for (UInt i = 0; i < scene->mNumMaterials; i++)
		{
			mMaterials[i] = Material(scene->mMaterials[i], std::to_string(i));
		}
	}
}

void Model::fillNodes(UInt& label, std::map<const aiNode*, UInt>& mapping, const aiNode* node)
{
	if (node != nullptr)
	{
		UInt currLabel = label;

		mapping[node] = label++;
		for (UInt i = 0; i < node->mNumChildren; i++)
		{
			fillNodes(label, mapping, node->mChildren[i]);
		}

		mNodes[currLabel] = Node(node, mapping);
		mNodeTransformations[currLabel] = toMat4(node->mTransformation);
	}
};

UInt Model::countNodes(const aiNode* node)
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


Mat4 toMat4(const aiMatrix4x4& mat)
{
	return Mat4(
		mat[0][0], mat[1][0], mat[2][0], mat[3][0],
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]
	);
}