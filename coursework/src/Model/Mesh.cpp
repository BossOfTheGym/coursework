#include "Model.h"


//===Mesh===
//constructors & destructors
Mesh::Mesh()
	: mName("")
	, mMaterialIndex(INVALID)
	, mVertexBuffer()
{}

Mesh::Mesh(const aiMesh* mesh) : Mesh()
{
	if (mesh)
	{
		mName = mesh->mName.C_Str();
		loadMesh(mesh);
	}
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

const UInt& Mesh::material() const
{
    return mMaterialIndex;
}


//private
void Mesh::loadMesh(const aiMesh* mesh)
{
	//get attributes
	auto[
		vertices
			, colors
			, normals
			, tangents
			, bitangents
			, textureCoords
	] = std::move(getAttributes(mesh));

	//all floats
	auto size =
		+vertices.size()
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

void Mesh::subData(GLuint attrib, GLint attribSize, GLint offset, GLsizei size, const float* data)
{
	mVertexBuffer.subData(offset, size * sizeof(float), data);

	mVertexBuffer.setAttribPointer(attrib, attribSize, GL_FLOAT, attribSize * sizeof(float), (const void*)offset);
	mVertexBuffer.enableAttribArray(attrib);
}

Mesh::Attributes Mesh::getAttributes(const aiMesh* mesh)
{
	std::vector<float> vertices;
	if (mesh->HasPositions())
	{
		vertices.reserve(3 * 3 * mesh->mNumFaces);

		float xMin = mesh->mVertices[0][0], xMax = mesh->mVertices[0][0];
		float yMin = mesh->mVertices[0][1], yMax = mesh->mVertices[0][1];
		float zMin = mesh->mVertices[0][2], zMax = mesh->mVertices[0][2];
		
		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			if (face.mNumIndices != 3)
			{
				std::cout << face.mNumIndices << std::endl;
			}
			for (UInt j = 0; j < face.mNumIndices; j++)
			{
				auto& index = face.mIndices[j];

				auto& vertex = mesh->mVertices[index];

				xMin = (xMin < vertex[0] ? xMin : vertex[0]);
				xMax = (xMax > vertex[0] ? xMax : vertex[0]);
				yMin = (yMin < vertex[1] ? yMin : vertex[1]);
				yMax = (yMax > vertex[1] ? yMax : vertex[1]);
				zMin = (zMin < vertex[2] ? zMin : vertex[2]);
				zMax = (zMax > vertex[2] ? zMax : vertex[2]);

				vertices.push_back(vertex[0]);
				vertices.push_back(vertex[1]);
				vertices.push_back(vertex[2]);
			}
		}

		float xDelta = xMax - xMin;
		float yDelta = yMax - yMin;
		float zDelta = zMax - zMin;
		float scale = (xDelta > yDelta ? xDelta : yDelta);
		scale = (scale > zDelta ? scale : zDelta);

		for (UInt i = 0; i < mesh->mNumFaces; i++)
		{
			vertices[9 * i + 0] /= scale;
			vertices[9 * i + 1] /= scale;
			vertices[9 * i + 2] /= scale;

			vertices[9 * i + 3] /= scale;
			vertices[9 * i + 4] /= scale;
			vertices[9 * i + 5] /= scale;

			vertices[9 * i + 6] /= scale;
			vertices[9 * i + 7] /= scale;
			vertices[9 * i + 8] /= scale;
		}
	}

	std::vector<float> colors;
	if (mesh->HasVertexColors(0))
	{
		colors.reserve(4 * 3 * mesh->mNumFaces);
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
		normals.reserve(3 * mesh->mNumFaces);
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
		tangents.reserve(3 * 3 * mesh->mNumFaces);
		bitangents.reserve(3 * 3 * mesh->mNumFaces);

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
		textureCoords.reserve(3 * 3 * mesh->mNumFaces);
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
		vertices
		, colors
		, normals
		, tangents
		, bitangents
		, textureCoords
	);
}
