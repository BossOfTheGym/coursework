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
#include <Model/Material.h>


class Mesh;
class Node;
class Model;


class Node : public IObjectBase
{
public:
    using UInt = unsigned int;
    using Indices = std::unique_ptr<UInt[]>;


public:
    Node();

    Node(std::map<const aiNode*, UInt>& mapping, const aiNode* node);

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


    Mat4& transform();

    const Mat4& transform() const;


private:
	Mat4 mTransform;

	UInt mNumChildren;
	Indices mChildren;

	UInt mNumMeshes;
	Indices mMeshes;

	String mName;
};



class Mesh : public IObjectBase
{
public:
	static const GLuint VERTEX = 0;
	static const GLuint COLOR = 1;
	static const GLuint NORMAL = 2;
	static const GLuint TANGENT = 3;
	static const GLuint BITANGENT = 4;
	static const GLuint TEXTURE = 5;

public:
    using UInt = unsigned int;
    using VAB = VertexArrayBuffer;
	using Attributes = std::tuple<
		  std::vector<float>
		, std::vector<float>
		, std::vector<float>
		, std::vector<float>
		, std::vector<float>
		, std::vector<float>
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
	void initVAB(const aiMesh* mesh);

	Attributes getAttributes(const aiMesh* mesh);

	void subData(GLuint attrib, GLint attribSize, GLint offset, GLsizei size, const float* data);


private:
	UInt mMaterialIndex;
	VAB mVertexBuffer;

	String mName;
};



class Model : public IObjectBase
{
public:
    using UInt = unsigned int;
    using Meshes   = std::unique_ptr<Mesh[]>;
    using Nodes    = std::unique_ptr<Node[]>;
    using Materials = std::unique_ptr<Texture2D[]>;

        
public:
    Model(const aiScene* scene = nullptr, const String& name = "");

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


    const Node& root() const;


private:
	bool loadModel(const aiScene* scene, const String& name);

private:
	UInt mNumMeshes;
	Meshes mMeshes;

	UInt mNumNodes;
	Nodes mNodes;

	UInt mNumMaterials;
	Materials mMaterials;

	String mName;
};