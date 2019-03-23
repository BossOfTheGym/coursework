#pragma once

#include <Common.h>

#include <Model/Base.h>



class PlanetNode : public INode
{
public:
	static const UInt MESH;

	static const UInt NUM_MESHES;

	static const UInt NUM_CHILDREN;


public:
	PlanetNode();

	PlanetNode(const String& name);

	PlanetNode(const PlanetNode& node) = delete;

	PlanetNode(PlanetNode&& node);


	virtual ~PlanetNode();


	PlanetNode& operator = (const PlanetNode& node) = delete;

	PlanetNode& operator = (PlanetNode&& node);


	//IObject
	virtual const String& toString() const override;


	//INode
	virtual const UInt& numChildren() const override;

	virtual const UInt* children() const override;


	virtual const UInt& numMeshes() const override;

	virtual const UInt* meshes() const override;


private:
	String mName;
};


class PlanetMesh : public IMesh
{
public:
	static const UInt MATERIAL;


public:
	PlanetMesh();

	PlanetMesh(UInt split, const String& name);

	PlanetMesh(const PlanetMesh& mesh) = delete;

	PlanetMesh(PlanetMesh&& mesh);


	virtual ~PlanetMesh();


	PlanetMesh& operator = (const PlanetMesh& mesh) = delete;

	PlanetMesh& operator = (PlanetMesh&& mesh);


	//IObject
	virtual const String& toString() const override;


	//IMesh
	virtual const VAB& vab() const override;

	virtual const UInt& material() const override;


private:
	void initMesh(UInt split);


private:
	VAB mVertexBuffer;

	String mName;
};


class PlanetMaterial : public IMaterial
{
public:
	static const UInt NUM_DIFFUSE;


public:
	PlanetMaterial();

	PlanetMaterial(
		  const String& diffuseLocation
		, const String& name
	);

	PlanetMaterial(const PlanetMaterial& material) = delete;

	PlanetMaterial(PlanetMaterial&& material);


	virtual ~PlanetMaterial();


	PlanetMaterial& operator = (const PlanetMaterial& material) = delete;

	PlanetMaterial& operator = (PlanetMaterial&& material);


	//IObject
	virtual const String& toString() const override;


	//IMaterial
	virtual const UInt& numDiffuse() const override;

	virtual const Texture2D** diffuse() const override;


private:
	Texture2D mDiffuse;
	Texture2D* mDiffusePtr;

	String mName;
};


class PlanetModel : public IModel
{
public:
	static const UInt NUM_MESHES;

	static const UInt NUM_NODES;

	static const UInt NUM_MATERIALS;


public:
	PlanetModel();

	PlanetModel(
		UInt split
		, const String& diffuseLocation
		, const String& name
	);

	PlanetModel(const PlanetModel& model) = delete;

	PlanetModel(PlanetModel&& model);


	virtual ~PlanetModel();


	PlanetModel& operator = (const PlanetModel& model) = delete;

	PlanetModel& operator = (PlanetModel&& model);


	//IObject
	virtual const String& toString() const override;


	//IModel
	virtual const UInt& numMeshes() const override;

	virtual const IMesh** meshes() const override;


	virtual const UInt& numNodes() const override;

	virtual const INode** nodes() const override;

	virtual const Mat4** transformations() const override;


	virtual const UInt& numMaterials() const override;

	virtual const IMaterial** materials() const override;


private:
	PlanetNode mNode;
	PlanetNode* mNodePtr;

	PlanetMesh mMesh;
	PlanetMesh* mMeshPtr;

	PlanetMaterial mMaterial;
	PlanetMaterial* mMaterialPtr;

	Mat4 mTransformation;
	Mat4* mTransformationPtr;

	String mName;
};


//utils
using Split    = std::vector<float>;
using DomeData = std::tuple<UInt, std::vector<float>>;

DomeData geoDome(UInt split);