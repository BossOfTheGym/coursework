#include "Model.h"


//===Model===
//constructors & destructor
Model::Model(const aiScene* scene, const String& name)
{
	if (scene)
	{
		if (!loadModel(scene, name))
		{
			//resetModel
		}
	}
}

Model::Model(Model&& model)
{
    //TODO
}


Model::~Model()
{}


//operators
Model& Model::operator = (Model&& model)
{
    if (this != &model)
    {
        //TODO
    }

    return *this;
}


//IObjectBase
const String& Model::toString() const
{
    return mName;
}


//get & set
const Model::UInt& Model::numMeshes() const
{
    return mNumMeshes;
}

const Model::Meshes& Model::meshes() const
{
    return mMeshes;
}


const Model::UInt& Model::numNodes() const
{
    return mNumNodes;
}

const Model::Nodes& Model::nodes() const
{
    return mNodes;
}


const Node& Model::root() const
{
    return mNodes[0];
}


//load
bool Model::loadModel(const aiScene* scene, const String& name)
{
	static std::function<UInt(const aiNode*)> countNodes
		= [&] (const aiNode* node) -> UInt
	{
		UInt count = 0;

		if (node != nullptr)
		{
			count = node->mNumChildren;
			for (UInt i = 0; i < node->mNumChildren; i++)
			{
				count += countNodes(node->mChildren[i]);
			}
		}

		return count;
	};

	static std::function<void(UInt&, std::map<const aiNode*, UInt>&, const aiNode*)> fillNodes
		= [&, this] (UInt& label, std::map<const aiNode*, UInt>& mapping, const aiNode* node) -> void
	{
		if (node != nullptr)
		{
			UInt currLabel = label;

			mapping[node] = label++;
			for (UInt i = 0; i < node->mNumChildren; i++)
			{
				fillNodes(label, mapping, node->mChildren[i]);
			}

			mNodes[currLabel] = Node(mapping, node);
		}
	};

	//name
	mName = name;

	//nodes
	UInt nodes = countNodes(scene->mRootNode);
	mNumNodes = nodes;
	mNodes.reset(new Node[nodes]);

	UInt label = 0;
	std::map<const aiNode*, UInt> mapping;
	fillNodes(label, mapping, scene->mRootNode);

	//meshes
	UInt meshes = scene->mNumMeshes;
	mNumMeshes = meshes;
	mMeshes.reset(new Mesh[meshes]);
	for (UInt i = 0; i < meshes; i++)
	{
		mMeshes[i] = Mesh(scene->mMeshes[i]);
	}

	//materials
	return true;
}