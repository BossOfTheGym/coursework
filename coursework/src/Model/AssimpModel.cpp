#include "AssimpModel.h"


//===Model===
//constructors & destructor
AssimpModel::AssimpModel()
	: mNumMeshes(0)
	, mMeshes(nullptr)
	, mMeshesPtrs(nullptr)
	, mNumNodes(0)
	, mNodes(nullptr)
	, mNodesPtrs(nullptr)
	, mNodeTransformations(nullptr)
	, mNodeTransformationsPtrs(nullptr)
	, mNumMaterials(0)
	, mMaterials(nullptr)
	, mMaterialsPtrs(nullptr)
	, mName("")
{}

AssimpModel::AssimpModel(const String& location) : AssimpModel()
{
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
		loadModel(scene, location);
	}
	else
	{
		std::cerr << "Failed to load model: " << location << std::endl;
	}
}

AssimpModel::AssimpModel(AssimpModel&& model)
{
	*this = std::move(model);
}


AssimpModel::~AssimpModel()
{}


//operators
AssimpModel& AssimpModel::operator = (AssimpModel&& model)
{
    if (this != &model)
    {
		std::swap(mNumMeshes, model.mNumMeshes);
		std::swap(mMeshes, model.mMeshes);
		std::swap(mMeshesPtrs, model.mMeshesPtrs);

		std::swap(mNumNodes, model.mNumNodes);
		std::swap(mNodes, model.mNodes);
		std::swap(mNodeTransformations, model.mNodeTransformations);
		std::swap(mNodesPtrs, model.mNodesPtrs);
		std::swap(mNodeTransformationsPtrs, model.mNodeTransformationsPtrs);

		std::swap(mNumMaterials, model.mNumMaterials);
		std::swap(mMaterials, model.mMaterials);
		std::swap(mMaterialsPtrs, model.mMaterialsPtrs);
    }

    return *this;
}


//IObjectBase
const String& AssimpModel::toString() const
{
    return mName;
}


//get & set
const UInt& AssimpModel::numMeshes() const
{
    return mNumMeshes;
}

const IMesh** AssimpModel::meshes() const
{
    return (const IMesh**)mMeshesPtrs.get();
}


const UInt& AssimpModel::numNodes() const
{
    return mNumNodes;
}

const INode** AssimpModel::nodes() const
{
    return (const INode**)mNodesPtrs.get();
}

const Mat4** AssimpModel::transformations() const
{
	return (const Mat4**)mNodeTransformationsPtrs.get();
}


const INode* AssimpModel::root() const
{
    return (mNodes ? &mNodes[0] : nullptr);
}


//load
void AssimpModel::loadModel(const aiScene* scene, const String& name)
{
	//name
	mName = name;

	//nodes
	loadNodes(scene);

	//meshes
	loadMeshes(scene);

	//materials
	loadMaterials(scene);
}

void AssimpModel::loadNodes(const aiScene* scene)
{
	mNumNodes = countNodes(scene->mRootNode);

	mNodes.reset(mNumNodes ? new AssimpNode[mNumNodes]() : nullptr);
	mNodeTransformations.reset(mNumNodes ? new Mat4[mNumNodes]() : nullptr);

	UInt label = 0;
	std::map<const aiNode*, UInt> mapping;
	fillNodes(label, mapping, scene->mRootNode);


	mNodesPtrs.reset(mNumNodes ? new AssimpNode*[mNumNodes] : nullptr);
	mNodeTransformationsPtrs.reset(mNumNodes ? new Mat4*[mNumNodes] : nullptr);
	for (UInt i = 0; i < mNumNodes; i++)
	{
		mNodesPtrs[i] = &mNodes[i];
		mNodeTransformationsPtrs[i] = &mNodeTransformations[i];
	}
}

void AssimpModel::loadMeshes(const aiScene* scene)
{
	if (scene->HasMeshes())
	{
		UInt meshes = scene->mNumMeshes;
		mNumMeshes = meshes;
		mMeshes.reset(mNumMeshes ? new AssimpMesh[meshes]() : nullptr);
		for (UInt i = 0; i < meshes; i++)
		{
			mMeshes[i] = AssimpMesh(scene->mMeshes[i]);
		}


		mMeshesPtrs.reset(mNumMeshes ? new AssimpMesh*[mNumMeshes] : nullptr);
		for (UInt i = 0; i < mNumMeshes; i++)
		{
			mMeshesPtrs[i] = &mMeshes[i];
		}
	}
}

void AssimpModel::loadMaterials(const aiScene* scene)
{
	if (scene->HasMaterials())
	{
		mNumMaterials = scene->mNumMaterials;
		mMaterials.reset(mNumMaterials ? new AssimpMaterial[mNumMaterials]() : nullptr);
		for (UInt i = 0; i < scene->mNumMaterials; i++)
		{
			mMaterials[i] = AssimpMaterial(scene->mMaterials[i], std::to_string(i));
		}


		mMaterialsPtrs.reset(mNumMaterials ? new AssimpMaterial*[mNumMaterials] : nullptr);
		for (UInt i = 0; i < mNumMaterials; i++)
		{
			mMaterialsPtrs[i] = &mMaterials[i];
		}
	}	
}


void AssimpModel::fillNodes(UInt& label, std::map<const aiNode*, UInt>& mapping, const aiNode* node)
{
	if (node != nullptr)
	{
		UInt currLabel = label;

		mapping[node] = label++;
		for (UInt i = 0; i < node->mNumChildren; i++)
		{
			fillNodes(label, mapping, node->mChildren[i]);
		}

		mNodes[currLabel] = AssimpNode(node, mapping);
		mNodeTransformations[currLabel] = toMat4(node->mTransformation);
	}
};

UInt AssimpModel::countNodes(const aiNode* node)
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