#pragma once

#include <functional>
#include <map>
#include <vector>


#include <assimp/scene.h>


#include <Common.h>

#include <Entity/IObject.h>

#include <Texture/Texture2D.h>

#include <Shader/ShaderProgram.h>

#include <Model/Model.h>
#include <Model/VertexArrayBuffer.h>


using UInt = unsigned int;

class Node;
class Mesh;
class Material;
class Model;


class Node : public IObjectBase
{
public:
    using Indices = std::unique_ptr<UInt[]>;


public:
	Node();

	Node(const aiNode* node, const std::map<const aiNode*, UInt>& mapping);

    Node(const Node& node) = delete;

    Node(Node&& node);


    ~Node();


	Node& operator = (const Node& node) = delete;

    Node& operator = (Node&& node);


    virtual const String& toString() const override;



    const UInt& numChildren() const;

    const Indices& children() const;


    const UInt& numMeshes() const;

    const Indices& meshes() const;


private:
	UInt mNumChildren;
	Indices mChildren;

	UInt mNumMeshes;
	Indices mMeshes;

	String mName;
};



class Mesh : public IObjectBase
{
public:
	static const UInt INVALID = -1;

	static const GLuint VERTEX    = 0;
	static const GLuint COLOR     = 1;
	static const GLuint NORMAL    = 2;
	static const GLuint TANGENT   = 3;
	static const GLuint BITANGENT = 4;
	static const GLuint TEXTURE   = 5;


public:
    using VAB = VertexArrayBuffer;
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


    ~Mesh();


    Mesh& operator = (const Mesh& mesh) = delete;

    Mesh& operator = (Mesh&& mesh);


    virtual const String& toString() const override;


    const VAB& vab() const;

    const UInt& material() const;


private:
	void loadMesh(const aiMesh* mesh);

	void subData(GLuint attrib, GLint attribSize, GLint offset, GLsizei size, const float* data);

	Attributes getAttributes(const aiMesh* mesh);


private:
	UInt mMaterialIndex;
	VAB mVertexBuffer;

	String mName;
};



class Material : public IObjectBase
{
public:
	using Textures = std::unique_ptr<Texture2D[]>;


public:
	Material();

	Material(const aiMaterial* material, const String& name);

	Material(const Material& material) = delete;

	Material(Material&& material);


	~Material();


	Material& operator = (const Material& material) = delete;

	Material& operator = (Material&& material);



	virtual const String& toString() const override;



	const UInt& numDiffuse(const String& path) const;

	const Textures& diffuse();


private:
	void loadMaterial(const aiMaterial* material);


private:
	UInt mNumDiffuse;
	Textures mDiffuse;

	String mName;
};



class Model : public IObjectBase
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

    const Meshes& meshes() const;
    

    const UInt& numNodes() const;

    const Nodes& nodes() const;

	const Transformations& transformations() const;


    const Node& root() const;


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