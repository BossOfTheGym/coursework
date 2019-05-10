#pragma once


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


#include <Common.h>

#include <glWrap/Texture/Texture2D.h>
#include <glWrap/Buffer/VertexArrayBuffer.h>


#include "Node.h"
#include "Material.h"
#include "Mesh.h"


class Model
{
public:
    using Meshes = std::vector<Mesh>;

    using Nodes = std::vector<Node>;

    using Materials = std::vector<Material>;

	using Transformations = std::vector<Mat4>;


public:
	Model();

	Model(
		Meshes&& meshes
		, Nodes&& nodes
		, Transformations&& nodesTransformations
		, Materials&& materials
	);

    Model(const Model& model) = delete;

    Model(Model&& model);


    ~Model();


    Model& operator = (const Model& model) = delete;

    Model& operator = (Model&& model);


	//Model
    const Meshes& meshes() const;
    
    const Nodes& nodes() const;

	const Transformations& transformations() const;

	const Materials& materials() const;


private:
	Meshes mMeshes;
	Nodes mNodes;
	Transformations mNodesTransformations;
	Materials mMaterials;
};


//aliases
using ModelShared = std::shared_ptr<Model>;
using ModelWeak = std::weak_ptr<Model>;