#include "Model.h"


//statics
namespace
{
	auto getAttributes(const aiMesh* mesh)
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
			  std::move(vertices)
			, std::move(colors)
			, std::move(normals)
			, std::move(tangents)
			, std::move(bitangents)
			, std::move(textureCoords)
		);
	}

	auto getVertexBuffer(const aiMesh* mesh)
	{
		//attributes
		auto[
			vertices
			, colors
			, normals
			, tangents
			, bitangents
			, textureCoords
		] = std::move(getAttributes(mesh));

		//all floats
		auto size = static_cast<GLsizei>(
			+ vertices.size()
			+ colors.size()
			+ normals.size()
			+ tangents.size()
			+ bitangents.size()
			+ textureCoords.size()
			);

		//vertices
		auto elements = static_cast<GLsizei>(3 * mesh->mNumFaces);

		//buffer offset
		auto offset = static_cast<int>(0);

		//init buffer
		VertexArrayBuffer vab(elements, size, nullptr);
		vab.bindArray();
		if (mesh->HasPositions())
		{
			vab.subData(offset, vertices.size() * sizeof(float), vertices.data());
			vab.setAttribPointer(Mesh::VAB::VERTEX, 3, GL_FLOAT, 3 * sizeof(float), (const void*)offset);

			offset += static_cast<int>(vertices.size() * sizeof(float));
		}

		if (mesh->HasVertexColors(0))
		{
			vab.subData(offset, colors.size() * sizeof(float), colors.data());
			vab.setAttribPointer(Mesh::VAB::COLOR, 4, GL_FLOAT, 4 * sizeof(float), (const void*)offset);

			offset += static_cast<int>(colors.size() * sizeof(float));
		}

		if (mesh->HasNormals())
		{
			vab.subData(offset, normals.size() * sizeof(float), normals.data());
			vab.setAttribPointer(Mesh::VAB::NORMAL, 3, GL_FLOAT, 3* sizeof(float), (const void*)offset);

			offset += static_cast<int>(normals.size() * sizeof(float));
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vab.subData(offset, tangents.size() * sizeof(float), tangents.data());
			vab.setAttribPointer(Mesh::VAB::TANGENT, 3, GL_FLOAT, 3 * sizeof(float), (const void*)offset);

			offset += static_cast<int>(tangents.size() * sizeof(float));


			vab.subData(offset, bitangents.size() * sizeof(float), bitangents.data());
			vab.setAttribPointer(Mesh::VAB::BITANGENT, 3, GL_FLOAT, 3 * sizeof(float), (const void*)offset);

			offset += static_cast<int>(bitangents.size() * sizeof(float));
		}

		if (mesh->HasTextureCoords(0))
		{
			vab.subData(offset, textureCoords.size() * sizeof(float), textureCoords.data());
			vab.setAttribPointer(Mesh::VAB::TEXTURE, 3, GL_FLOAT, 3 * sizeof(float), (const void*)offset);

			offset += static_cast<int>(textureCoords.size() * sizeof(float));
		}

		return vab;
	}
}

Mesh Mesh::buildFromAssimp(const aiMesh* mesh)
{
	Mesh result;

	if (mesh)
	{
		//name
		result.mName = mesh->mName.C_Str();

		//material
		result.mMaterialIndex = mesh->mMaterialIndex;

		//buffer
		result.mVertexBuffer = std::move(getVertexBuffer(mesh));
	}

	return result;
}

Mesh Mesh::buildFromData()
{
	return Mesh();
}


//constructors & destructors
Mesh::Mesh()
	: mName("")
	, mMaterialIndex(VAB::INVALID)
	, mVertexBuffer()
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


//IObject
const String& Mesh::toString() const
{
    return mName;
}


//IMesh
const Mesh::VAB& Mesh::vab() const
{
    return mVertexBuffer;
}

const UInt& Mesh::material() const
{
    return mMaterialIndex;
}
