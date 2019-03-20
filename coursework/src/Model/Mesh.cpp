#include "Model.h"


//===Mesh===
//constructors & destructors
Mesh::Mesh()
	: mName("")
	, mVertexBuffer()
	, mMaterialIndex()
{}

Mesh::Mesh(const aiMesh* mesh)
	: mName(mesh->mName.C_Str())
{
    //get attributes
	auto[ 
		vertices
		, colors
		, normals
		, tangents
		, bitangents
		, textureCoords
	] = getAttributes(mesh);

	//all floats
	auto size =
		+ vertices.size()
		+ colors.size()
		+ normals.size()
		+ tangents.size()
		+ bitangents.size()
		+ textureCoords.size();

	//vertices
	auto elements = 3 * mesh->mNumFaces;

	//buffer offset
	auto offset = 0;

	//init buffer
	GLint prevArray;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevArray);

	mVertexBuffer = VAB(elements, size, nullptr);
	mVertexBuffer.bindArray();
	if (mesh->HasPositions())
	{
		subData(VERTEX, 3, offset, vertices.size(), vertices.data());

		offset += vertices.size() * sizeof(float);
	}

	if (mesh->HasVertexColors(0))
	{	
		subData(COLOR, 4, offset, colors.size(), colors.data());

		offset += colors.size() * sizeof(float);
	}

	if (mesh->HasNormals())
	{
		subData(NORMAL, 3, offset, normals.size(), normals.data());

		offset += normals.size() * sizeof(float);
	}

	if (mesh->HasTangentsAndBitangents())
	{
		subData(TANGENT, 3, offset, tangents.size(), tangents.data());

		offset += tangents.size() * sizeof(float);


		subData(BITANGENT, 3, offset, bitangents.size(), bitangents.data());

		offset += bitangents.size() * sizeof(float);
	}

	if (mesh->HasTextureCoords(0))
	{
		subData(TEXTURE, 3, offset, textureCoords.size(), textureCoords.data());

		offset += textureCoords.size() * sizeof(float);
	}
	glBindVertexArray(prevArray);


    //materials
    mMaterialIndex = mesh->mMaterialIndex;
}

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
		mMaterialIndex = mesh.mMaterialIndex;
		mVertexBuffer = std::move(mesh.mVertexBuffer);
		mName = std::move(mesh.mName);
    }

    return *this;
}


//IObjectBase
const String& Mesh::toString() const
{
    return mName;
}


//get & set
const Mesh::VAB& Mesh::vab() const
{
    return mVertexBuffer;
}

const Mesh::UInt& Mesh::material() const
{
    return mMaterialIndex;
}


//private
auto&& Mesh::getAttributes(const aiMesh* mesh)
{
	std::vector<float> vertices;
	if (mesh->HasPositions())
	{
		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			for (UInt j = 0; j < face.mNumIndices; j++)
			{
				auto& index = face.mIndices[j];

				auto& vertex = mesh->mVertices[index];

				vertices.push_back(vertex[0]);
				vertices.push_back(vertex[1]);
				vertices.push_back(vertex[2]);
			}
		}
	}

	std::vector<float> colors;
	if (mesh->HasVertexColors(0))
	{
		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			for (UInt j = 0; j < face.mNumIndices; j++)
			{
				auto& index = face.mIndices[j];

				auto& color = mesh->mColors[0][index];
				colors.push_back(color.r);
				colors.push_back(color.g);
				colors.push_back(color.b);
				colors.push_back(color.a);
			}
		}
	}

	std::vector<float> normals;
	if (mesh->HasNormals())
	{
		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			for (UInt j = 0; j < face.mNumIndices; j++)
			{
				auto& index = face.mIndices[j];

				auto& normal = mesh->mNormals[index];
				normals.push_back(normal[0]);
				normals.push_back(normal[1]);
				normals.push_back(normal[2]);
			}
		}
	}

	std::vector<float> tangents;
	std::vector<float> bitangents;
	if (mesh->HasTangentsAndBitangents())
	{
		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			for (UInt j = 0; j < face.mNumIndices; j++)
			{
				auto& index = face.mIndices[j];

				auto& tangent = mesh->mTangents[index];
				tangents.push_back(tangent[0]);
				tangents.push_back(tangent[1]);
				tangents.push_back(tangent[2]);

				auto& bitangent = mesh->mBitangents[index];
				bitangents.push_back(bitangent[0]);
				bitangents.push_back(bitangent[1]);
				bitangents.push_back(bitangent[2]);
			}
		}
	}

	std::vector<float> textureCoords;
	if (mesh->HasTextureCoords(0))
	{
		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			for (UInt j = 0; j < face.mNumIndices; j++)
			{
				auto& index = face.mIndices[j];

				auto& tex = mesh->mTextureCoords[0][index];
				textureCoords.push_back(tex.x);
				textureCoords.push_back(tex.y);
				textureCoords.push_back(tex.z);
			}
		}
	}

	return std::make_tuple(
		std::move(vertices)
		, std::move(colors)
		, std::move(normals)
		, std::move(tangents)
		, std::move(bitangents)
		, std::move(textureCoords)
	);
}

void Mesh::subData(GLuint attrib, GLint attribSize, GLint offset, GLsizei size, const float* data)
{
	mVertexBuffer.subData(offset, size * sizeof(float), data);

	mVertexBuffer.setAttribPointer(attrib, attribSize, GL_FLOAT, attribSize * sizeof(float), (const void*)offset);
	mVertexBuffer.enableAttribArray(attrib);
}
