#pragma once

#include <functional>
#include <map>
#include <vector>


#include <assimp/scene.h>


#include <Common.h>

#include <Entity/IObject.h>

#include <Texture/Texture2D.h>

#include <Shader/ShaderProgram.h>

#include <Model/Base.h>
#include <Model/Model.h>
#include <Model/VertexArrayBuffer.h>


class Node;
class Mesh;
class Material;
class Model;


class Node : public INode
{
public:
	using Indices = std::unique_ptr<UInt[]>;


public:
	Node();

	Node(const aiNode* node, const std::map<const aiNode*, UInt>& mapping);

    Node(const Node& node) = delete;

    Node(Node&& node);


    virtual ~Node();


	Node& operator = (const Node& node) = delete;

    Node& operator = (Node&& node);


    virtual const String& toString() const override;



    virtual const UInt& numChildren() const override;

    virtual const UInt* children() const override;


    virtual const UInt& numMeshes() const override;

    virtual const UInt* meshes() const override;


private:
	UInt mNumChildren;
	Indices mChildren;

	UInt mNumMeshes;
	Indices mMeshes;

	String mName;
};


class Mesh : public IMesh
{
public:
	using Attributes = std::tuple<
		  std::vector<float> // vertex
		, std::vector<float> // color
		, std::vector<float> // normal
		, std::vector<float> // tangent
		, std::vector<float> // bitangent
		, std::vector<float> // texture
	>;


public:
	Mesh();

    Mesh(const aiMesh* mesh);

    Mesh(const Mesh& mesh) = delete;

    Mesh(Mesh&& mesh);


    virtual ~Mesh();


    Mesh& operator = (const Mesh& mesh) = delete;

    Mesh& operator = (Mesh&& mesh);


    virtual const String& toString() const override;


    virtual const VAB& vab() const override;

    virtual const UInt& material() const override;


private:
	void loadMesh(const aiMesh* mesh);

	void subData(GLuint attrib, GLint attribSize, GLint offset, GLsizei size, const float* data);

	Attributes getAttributes(const aiMesh* mesh);


private:
	UInt mMaterialIndex;
	VAB mVertexBuffer;

	String mName;
};


class Material : public IMaterial
{
public:
	using TexturesPtr = std::unique_ptr<Texture2D[]>;


public:
	Material();

	Material(const aiMaterial* material, const String& name);

	Material(const Material& material) = delete;

	Material(Material&& material);


	virtual ~Material();


	Material& operator = (const Material& material) = delete;

	Material& operator = (Material&& material);



	virtual const String& toString() const override;



	virtual const UInt& numDiffuse() const override;

	virtual const Texture2D* diffuse() const override;


private:
	void loadMaterial(const aiMaterial* material);


private:
	UInt mNumDiffuse;
	TexturesPtr mDiffuse;

	String mName;
};


class Model : public IModel
{
public:
    using Meshes    = std::unique_ptr<Mesh[]>;
    using Nodes     = std::unique_ptr<Node[]>;
    using Materials = std::unique_ptr<Material[]>;
	using Transformations = std::unique_ptr<Mat4[]>;

        
public:
	Model();

    Model(const aiScene* scene, const String& name);

    Model(const Model& model) = delete;

    Model(Model&& model);


    virtual ~Model();


    Model& operator = (const Model& model) = delete;

    Model& operator = (Model&& model);



    virtual const String& toString() const override;



    const UInt& numMeshes() const;

    const IMesh* meshes() const;
    

    const UInt& numNodes() const;

    const INode* nodes() const;

	const Mat4* transformations() const;


    const INode& root() const;


private:
	void loadModel(const aiScene* scene, const String& name);

	void fillNodes(UInt& label, std::map<const aiNode*, UInt>& mapping, const aiNode* node);

	UInt countNodes(const aiNode* node);


private:
	UInt mNumMeshes;
	Meshes mMeshes;

	UInt mNumNodes;
	Nodes mNodes;
	Transformations mNodeTransformations;

	UInt mNumMaterials;
	Materials mMaterials;

	String mName;
};


//util
Mat4 toMat4(const aiMatrix4x4& mat);