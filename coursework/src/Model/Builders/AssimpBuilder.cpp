#include "AssimpBuilder.h"


AssimpBuilder::AssimpBuilder()
	: mImported()
	, mImporter()
	, mImportFlags(DEFAULT_FLAGS)
{}

AssimpBuilder::~AssimpBuilder()
{}


int& AssimpBuilder::flags()
{
	return mImportFlags;
}

AssimpBuilder::Importer& AssimpBuilder::importer()
{
	return mImporter;
}

Model&& AssimpBuilder::imported()
{
	return std::move(mImported);
}



namespace
{
	using NodeMapping = std::map<const aiNode*, UInt>;



	auto toMat4(const aiMatrix4x4& mat)
	{
		return Mat4(
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3]
		);
	}

	auto loadNode(const aiNode* node, const NodeMapping& mapping)
	{
		using Indices = Node::Indices;

		String name = "";
		Indices children;
		Indices meshes;

		if (node)
		{
			//name
			String name = node->mName.C_Str();

			//node children
			children.resize(node->mNumChildren);
			for (UInt i = 0; i < node->mNumChildren; i++)
			{
				auto it = mapping.find(node->mChildren[i]);
				children[i] = it->second;
			}

			//node meshes
			meshes.resize(node->mNumMeshes);
			for (UInt i = 0; i < node->mNumMeshes; i++)
			{
				meshes[i] = node->mMeshes[i];
			}
		}

		return Node(std::move(children), std::move(meshes), std::move(name));
	}



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

			float xCenter = (xMax + xMin) / 2.0f;
			float yCenter = (yMax + yMin) / 2.0f;
			float zCenter = (zMax + zMin) / 2.0f;

			for (UInt i = 0; i < mesh->mNumFaces; i++)
			{
				vertices[9 * i + 0] = (vertices[9 * i + 0] - xCenter) / scale;
				vertices[9 * i + 1] = (vertices[9 * i + 1] - yCenter) / scale;
				vertices[9 * i + 2] = (vertices[9 * i + 2] - zCenter) / scale;

				vertices[9 * i + 3] = (vertices[9 * i + 3] - xCenter) / scale;
				vertices[9 * i + 4] = (vertices[9 * i + 4] - yCenter) / scale;
				vertices[9 * i + 5] = (vertices[9 * i + 5] - zCenter) / scale;

				vertices[9 * i + 6] = (vertices[9 * i + 6] - xCenter) / scale;
				vertices[9 * i + 7] = (vertices[9 * i + 7] - yCenter) / scale;
				vertices[9 * i + 8] = (vertices[9 * i + 8] - zCenter) / scale;
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

	auto loadMesh(const aiMesh* mesh)
	{
		using VAB = Mesh::VAB;

		String name;
		VAB vertexBuffer;
		UInt materialIndex;

		if (mesh)
		{
			//name
			name = mesh->mName.C_Str();

			//material
			materialIndex = mesh->mMaterialIndex;

			//buffer
			vertexBuffer = std::move(getVertexBuffer(mesh));
		}

		return Mesh(std::move(vertexBuffer), std::move(materialIndex), std::move(name));
	}


	
	auto loadMaterial(const aiMaterial* material, const String& name)
	{
		using Textures = Material::Textures;

		Textures diffuse(material->GetTextureCount(aiTextureType_DIFFUSE));

		//diffuse
		aiString path;
		for (UInt i = 0; i < diffuse.size(); i++)
		{
			if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS)
			{
				diffuse[i] = Texture2D(path.C_Str());
			}
		}

		return Material(std::move(diffuse), String(name));
	}


	
	auto loadNodes(const aiScene* scene)
	{
		using Nodes = Model::Nodes;
		using Transformations = Model::Transformations;


		Nodes nodes;
		Transformations nodesTransformations;


		//count nodes
		std::function<UInt(const aiNode*)> countNodes = 
			[&] (const aiNode* node) -> UInt
		{
			UInt count = 0;

			if (node != nullptr)
			{
				//count self
				count = 1;
				for (UInt i = 0; i < node->mNumChildren; i++)
				{
					count += countNodes(node->mChildren[i]);
				}
			}

			return count;
		};

		UInt count = countNodes(scene->mRootNode);
		nodes.resize(count);
		nodesTransformations.resize(count);


		//fill nodes data
		std::function<void(UInt&, NodeMapping&, const aiNode*)> fillNodes
			= [&] (UInt& label, NodeMapping& mapping, const aiNode* node) -> void
		{
			if (node != nullptr)
			{
				UInt currLabel = label;

				mapping[node] = label++;
				for (UInt i = 0; i < node->mNumChildren; i++)
				{
					fillNodes(label, mapping, node->mChildren[i]);
				}

				nodes[currLabel] = std::move(loadNode(node, mapping));
				nodesTransformations[currLabel] = toMat4(node->mTransformation);
			}
		};

		UInt label = 0;
		NodeMapping mapping;
		fillNodes(label, mapping, scene->mRootNode);


		return std::make_tuple(std::move(nodes), std::move(nodesTransformations));
	}

	

	auto loadMeshes(const aiScene* scene)
	{
		Model::Meshes meshes;

		if (scene->HasMeshes())
		{
			meshes.resize(scene->mNumMeshes);
			for (UInt i = 0; i < meshes.size(); i++)
			{
				meshes[i] = std::move(loadMesh(scene->mMeshes[i]));
			}
		}

		return meshes;
	}

	

	auto loadMaterials(const aiScene* scene)
	{
		Model::Materials materials;

		if (scene->HasMaterials())
		{
			materials.resize(scene->mNumMaterials);
			for (UInt i = 0; i < scene->mNumMaterials; i++)
			{
				materials[i] = std::move(loadMaterial(scene->mMaterials[i], std::to_string(i)));
			}
		}	

		return materials;
	}
}


void AssimpBuilder::readFile(const String& location)
{
	using Nodes = Model::Nodes;
	using Transformations = Model::Transformations;
	using Meshes = Model::Meshes;
	using Materials = Model::Materials;


	const aiScene* scene = mImporter.ReadFile(location, mImportFlags);

	String name;
	Nodes nodes;
	Transformations nodesTransformations;
	Meshes meshes;
	Materials materials;

	if (scene)
	{
		//name
		name = location;

		//nodes data
		std::tie(nodes, nodesTransformations) = std::move(loadNodes(scene));

		//meshes
		meshes = std::move(loadMeshes(scene));

		//materials
		materials = std::move(loadMaterials(scene));
	}
	else
	{
		std::cerr << "Failed to load model: " << location << std::endl;
	}

	mImported = std::move(
		Model(
			std::move(meshes)
			, std::move(nodes)
			, std::move(nodesTransformations)
			, std::move(materials)
			, std::move(name)
		)
	);
}