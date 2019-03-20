#include "Model.h"


//===Mesh===
//constructors & destructors
Mesh::Mesh()
{
	//TODO
}

Mesh::Mesh(const aiMesh* mesh)
{
	//TODO

    //name
    mName = mesh->mName.C_Str();

    //fill
    std::vector<float> vertices;
    if (mesh->HasPositions())
    {
        for (UInt i = 0; i < mesh->mNumFaces; i++)
        {
            auto& face = mesh->mFaces[i];

            for (UInt j = 0; j < face.mNumIndices; j++)
            {
                auto& index  = face.mIndices[j];

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
                auto& index  = face.mIndices[j];

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

    }
    


	std::vector<float> data;
	if (mesh->HasPositions())
	{
		data.insert(data.end(), vertices.begin(), vertices.end());
	}

	if (mesh->HasVertexColors(0))
	{
		data.insert(data.end(), colors.begin(), colors.end());
	}

	if (mesh->HasNormals())
	{
		data.insert(data.end(), normals.begin(), normals.end());
	}

	if (mesh->HasTangentsAndBitangents())
	{
		data.insert(data.end(), tangents.begin(), tangents.end());
		data.insert(data.end(), bitangents.begin(), bitangents.end());
	}

	if (mesh->HasTextureCoords(0))
	{

	}
	

    //materials
    mMaterialIndex = mesh->mMaterialIndex;
}

Mesh::Mesh(Mesh&& mesh)
{
	//TODO
}


Mesh::~Mesh()
{}


//operators
Mesh& Mesh::operator = (Mesh&& mesh)
{
    if (this != &mesh)
    {
        //TODO
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