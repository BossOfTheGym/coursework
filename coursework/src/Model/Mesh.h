#pragma once

#include <assimp/scene.h>

#include <Common.h>

#include <glWrap/Buffer/VertexArrayBuffer.h>


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