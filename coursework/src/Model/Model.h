#pragma once


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


#include <Common.h>

#include <Texture/Texture2D.h>
#include <Model/VertexArrayBuffer.h>


class Node
{
public:
	using Indices = std::vector<UInt>;


public:
	Node();

	Node(Indices&& children, Indices&& meshes, String&& name);

    Node(const Node& node) = delete;

    Node(Node&& node);


    ~Node();


	Node& operator = (const Node& node) = delete;

    Node& operator = (Node&& node);


	//Node
    const Indices& children() const;

    const Indices& meshes() const;

	const String& name() const;


private:
	Indices mChildren;
	Indices mMeshes;

	String mName;
};


class Mesh
{
public:
	using VAB = VertexArrayBuffer;


public:
	Mesh();

	Mesh(VAB&& vertexBuffer, UInt&& materialIndex, String&& name);

    Mesh(const Mesh& mesh) = delete;

    Mesh(Mesh&& mesh);


    ~Mesh();


    Mesh& operator = (const Mesh& mesh) = delete;

    Mesh& operator = (Mesh&& mesh);


	//Mesh
	const VAB& vab() const;

	const UInt& material() const;

	const String& name() const;


private:
	VAB mVertexBuffer;
	UInt mMaterialIndex;

	String mName;
};


class Material
{
public:
	using Textures = std::vector<Texture2D>;


public:
	Material();

	Material(Textures&& diffuse);

	Material(const Material& material) = delete;

	Material(Material&& material);


	~Material();


	Material& operator = (const Material& material) = delete;

	Material& operator = (Material&& material);


	//Material
	const Textures& diffuse() const;


private:
	Textures mDiffuse;
};


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