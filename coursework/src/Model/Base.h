#pragma once

#include <Entity/IObject.h>

#include <Texture/Texture2D.h>
#include <Model/VertexArrayBuffer.h>


using UInt = unsigned int;


class IMaterial;
class IMesh;
class INode;
class IModel;


class INode : public IObject
{
public:
	virtual const UInt& numChildren() const = 0;

	virtual const UInt* children() const = 0;


	virtual const UInt& numMeshes() const = 0;

	virtual const UInt* meshes() const = 0;
};


class IMesh : public IObject
{
public:
	enum Attributes : GLuint
	{
		  INVALID   = GLuint(-1)
		, VERTEX    = GLuint(0)
		, COLOR     = GLuint(1)
		, NORMAL    = GLuint(2)
		, TANGENT   = GLuint(3)
		, BITANGENT = GLuint(4)
		, TEXTURE   = GLuint(5)
	};

	using VAB = VertexArrayBuffer;


public:
	virtual const VAB& vab() const = 0;

	virtual const UInt& material() const = 0;
};


class IMaterial : public IObject
{
public:
	virtual const UInt& numDiffuse() const = 0;

	virtual const Texture2D** diffuse() const = 0;
};


class IModel : public IObject
{
public:
	virtual const UInt& numMeshes() const = 0;

	virtual const IMesh** meshes() const = 0;


	virtual const UInt& numNodes() const = 0;

	virtual const INode** nodes() const = 0;

	virtual const Mat4** transformations() const = 0;


	virtual const INode* root() const = 0;
};