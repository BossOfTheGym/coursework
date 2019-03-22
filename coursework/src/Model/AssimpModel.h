#pragma once


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


#include <Common.h>

#include <Entity/IObject.h>

#include <Texture/Texture2D.h>

#include <Shader/ShaderProgram.h>

#include <Model/Base.h>
#include <Model/VertexArrayBuffer.h>


class AssimpNode;
class AssimpMesh;
class AssimpMaterial;
class AssimpModel;


class AssimpNode : public INode
{
public:
	using Indices = std::unique_ptr<UInt[]>;


public:
	AssimpNode();

	AssimpNode(const aiNode* node, const std::map<const aiNode*, UInt>& mapping);

    AssimpNode(const AssimpNode& node) = delete;

    AssimpNode(AssimpNode&& node);


    virtual ~AssimpNode();


	AssimpNode& operator = (const AssimpNode& node) = delete;

    AssimpNode& operator = (AssimpNode&& node);


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


class AssimpMesh : public IMesh
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
	AssimpMesh();

    AssimpMesh(const aiMesh* mesh);

    AssimpMesh(const AssimpMesh& mesh) = delete;

    AssimpMesh(AssimpMesh&& mesh);


    virtual ~AssimpMesh();


    AssimpMesh& operator = (const AssimpMesh& mesh) = delete;

    AssimpMesh& operator = (AssimpMesh&& mesh);


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


class AssimpMaterial : public IMaterial
{
public:
	using TexturesPtr = std::unique_ptr<Texture2D[]>;


public:
	AssimpMaterial();

	AssimpMaterial(const aiMaterial* material, const String& name);

	AssimpMaterial(const AssimpMaterial& material) = delete;

	AssimpMaterial(AssimpMaterial&& material);


	virtual ~AssimpMaterial();


	AssimpMaterial& operator = (const AssimpMaterial& material) = delete;

	AssimpMaterial& operator = (AssimpMaterial&& material);



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


class AssimpModel : public IModel
{
public:
    using Meshes    = std::unique_ptr<AssimpMesh[]>;
    using Nodes     = std::unique_ptr<AssimpNode[]>;
    using Materials = std::unique_ptr<AssimpMaterial[]>;
	using Transformations = std::unique_ptr<Mat4[]>;

        
public:
	AssimpModel();

    AssimpModel(const String& location);

    AssimpModel(const AssimpModel& model) = delete;

    AssimpModel(AssimpModel&& model);


    virtual ~AssimpModel();


    AssimpModel& operator = (const AssimpModel& model) = delete;

    AssimpModel& operator = (AssimpModel&& model);



    virtual const String& toString() const override;



    const UInt& numMeshes() const;

    const IMesh* meshes() const;
    

    const UInt& numNodes() const;

    const INode* nodes() const;

	const Mat4* transformations() const;


    const INode* root() const;


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