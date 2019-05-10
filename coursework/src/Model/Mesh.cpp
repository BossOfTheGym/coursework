#include "Mesh.h"


//constructors & destructors
Mesh::Mesh()
	: mName("")
	, mMaterialIndex(-1)
	, mVertexBuffer()
{}

Mesh::Mesh(VAB&& vertexBuffer, UInt&& materialIndex, String&& name)
	: mVertexBuffer(std::move(vertexBuffer))
	, mMaterialIndex(std::move(materialIndex))
	, mName(std::move(name))
{}

Mesh::Mesh(Mesh&& mesh)
{
	*this = std::move(mesh);
}


Mesh::~Mesh()
{}


//operators
Mesh& Mesh::operator = (Mesh&& mesh)
{
    if (this != &mesh)
    {
		std::swap(mMaterialIndex, mesh.mMaterialIndex);		
		std::swap(mVertexBuffer, mesh.mVertexBuffer);

		std::swap(mName, mesh.mName);
    }

    return *this;
}


const String& Mesh::name() const
{
    return mName;
}


const Mesh::VAB& Mesh::vab() const
{
    return mVertexBuffer;
}

const UInt& Mesh::material() const
{
    return mMaterialIndex;
}
