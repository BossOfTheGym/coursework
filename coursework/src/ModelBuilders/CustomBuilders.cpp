#include "CustomBuilders.h"

#include "Attributes.h"


//PlanetBuilder
PlanetBuilder::PlanetBuilder()
	: mBuilt()
{}

PlanetBuilder::~PlanetBuilder()
{}


namespace
{
	auto buildIdentityTransform()
	{
		return Mat4(1.0f);
	}

	auto buildSingleNode(const String& name)
	{
		using Indices = Node::Indices;

		Indices children{};

		Indices meshes{0};

		return Node(std::move(children), std::move(meshes), String(name));
	}
	


	void pushVertex(std::vector<double>& data, const Vec3& vertex)
	{
		data.push_back(vertex.x);
		data.push_back(vertex.y);
		data.push_back(vertex.z);
	}

	void pushSplitedTriangles(
		std::vector<double>& data 
		, const Vec3& vertex0
		, const Vec3& vertex1
		, const Vec3& vertex2
		, UInt split)
	{
		if (split > 0)
		{
			Vec3 vertex01 = (vertex0 + vertex1) / 2.0;
			Vec3 vertex12 = (vertex1 + vertex2) / 2.0;
			Vec3 vertex20 = (vertex2 + vertex0) / 2.0;

			pushSplitedTriangles(data, vertex0, vertex01, vertex20, split - 1);

			pushSplitedTriangles(data, vertex01, vertex1, vertex12, split - 1);

			pushSplitedTriangles(data, vertex12, vertex2, vertex20, split - 1);

			pushSplitedTriangles(data, vertex01, vertex12, vertex20, split - 1);
		}
		else
		{
			pushVertex(data, glm::normalize(vertex0));
			pushVertex(data, glm::normalize(vertex1));
			pushVertex(data, glm::normalize(vertex2));
		}
	}
	
	auto sphere(UInt split)
	{
		//icosahedron params
		const int PENTAGON_SIDES = 5;

		const double HALF_ANGLE = PI / PENTAGON_SIDES;
		const double ANGLE      = 2 * HALF_ANGLE;
		const double SIN_HALF   = sin(HALF_ANGLE);

		const double ICOSAHEDRON_SIDE = sqrt(4 * pow(SIN_HALF, 2) - 1.0f) / SIN_HALF;
		const double PYRAMID_HEIGHT   = pow(ICOSAHEDRON_SIDE, 2) / 2;
		const double RADIUS           = ICOSAHEDRON_SIDE / (2 * SIN_HALF);
		const double HEIGHT_ANGLE = atan((1.0f - PYRAMID_HEIGHT) / RADIUS);

		const double A   = ICOSAHEDRON_SIDE;
		const double H   = PYRAMID_HEIGHT;
		const double R   = RADIUS;
		const double PHI = HEIGHT_ANGLE;

		const int N = PENTAGON_SIDES;


		//init icosahedron vertices
		glm::vec3 top    = glm::vec3(0.0f, +1.0f, 0.0f);
		glm::vec3 bottom = glm::vec3(0.0f, -1.0f, 0.0f);

		glm::vec3    topRing[N];
		glm::vec3 bottomRing[N];


		double angle;
		for (int i = 0; i < N; i++) 
		{
			angle = ANGLE * i;
			topRing[i] = glm::vec3(R * cos(angle), 1.0f - H, R * sin(angle));

			angle = ANGLE * i + HALF_ANGLE;
			bottomRing[i] = glm::vec3(R * cos(angle), H - 1.0f, R * sin(angle));	
		}


		//assemble triangles
		std::vector<double> data;
		for (int curr = 0; curr < N; curr++)
		{
			int prev = (curr + N - 1) % N;
			int next = (curr + N + 1) % N;

			//top + topRing[i] + topRing[i + 1]
			pushSplitedTriangles(data, top, topRing[next], topRing[curr], split);

			//bottom + bottomRing[i] + bottomRing[i + 1]
			pushSplitedTriangles(data, bottom, bottomRing[curr], bottomRing[next], split);

			//bottomRing[i] + topRing[i] + topRing[i + 1]
			pushSplitedTriangles(data, bottomRing[curr], topRing[curr], topRing[next], split);;

			//topRing[i] + bottomRing[i] + bottomRing[i - 1]
			pushSplitedTriangles(data, topRing[curr], bottomRing[curr], bottomRing[prev], split);
		}

		return std::make_tuple(static_cast<UInt>(data.size() / 3), std::move(data));
	}

	auto buildSphereMesh(UInt split, const String& name)
	{
		using VAB = VertexArrayBuffer;


		auto[elements, data] = std::move(sphere(split));

		VAB buffer = std::move(
			VertexArrayBuffer(
				elements
				, static_cast<GLsizei>(data.size())
				, data.data()
			)
		);
		buffer.bind();
		buffer.setAttribPointer(static_cast<GLuint>(Attributes::VERTEX), 3, GL_DOUBLE, 3 * sizeof(double), 0);

		UInt materialIndex = 0;
			
		return Mesh(std::move(buffer), UInt(materialIndex), String(name));
	}
	


	auto buildMaterial(const String& diffuseLocation)
	{
		using Textures = Material::Textures;


		Textures diffuse(1);
		diffuse[0] = std::move(Texture2D(diffuseLocation));

		//should be configurable
		diffuse[0].bind();
		diffuse[0].texParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
		diffuse[0].texParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
		diffuse[0].texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		diffuse[0].texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		return Material(std::move(diffuse));
	}
}

void PlanetBuilder::build(
	UInt triangleSplit
	, const String& diffuseLocation
	, const String& name
)
{
	using Meshes = Model::Meshes;
	using Nodes  = Model::Nodes;
	using Transformations = Model::Transformations;
	using Materials = Model::Materials;


	Nodes nodes(1);
	nodes[0] = std::move(buildSingleNode(name));

	Meshes meshes(1);
	meshes[0] = std::move(buildSphereMesh(triangleSplit, name));

	Materials materials(1);
	materials[0] = std::move(buildMaterial(diffuseLocation));

	Transformations transformations(1);
	transformations[0] = std::move(buildIdentityTransform());

	mBuilt = std::move(
		Model(
			std::move(meshes)
			, std::move(nodes)
			, std::move(transformations)
			, std::move(materials)
		)
	);
}


Model&& PlanetBuilder::model()
{
	return std::move(mBuilt);
}



//BoxBuilder
BoxBuilder::BoxBuilder()
	: mBuilt()
{}

BoxBuilder::~BoxBuilder()
{}



namespace
{
	auto box()
	{
		static const Vec3 BOX_VERTICES[8] = {
			  Vec3(-1.0f, -1.0f, -1.0f)
			, Vec3(-1.0f, -1.0f, +1.0f)
			, Vec3(-1.0f, +1.0f, -1.0f)
			, Vec3(-1.0f, +1.0f, +1.0f)
			, Vec3(+1.0f, -1.0f, -1.0f)
			, Vec3(+1.0f, -1.0f, +1.0f)
			, Vec3(+1.0f, +1.0f, -1.0f)
			, Vec3(+1.0f, +1.0f, +1.0f)
		};

		static const Vec3 NORMALS[6] = {
			  Vec3(-1.0f, +0.0f, +0.0f)
			, Vec3(+0.0f, +0.0f, -1.0f)
			, Vec3(+1.0f, +0.0f, +0.0f)
			, Vec3(+0.0f, +0.0f, +1.0f)
			, Vec3(+0.0f, +1.0f, +0.0f)
			, Vec3(+0.0f, -1.0f, +0.0f)
		};

		static const int INDICES[12][4] = {
			  1, 3, 2, 0
			, 2, 0, 1, 0

			, 0, 2, 6, 1
			, 6, 4, 0, 1

			, 4, 6, 7, 2
			, 7, 5, 4, 2

			, 5, 7, 3, 3
			, 3, 1, 5, 3

			, 7, 6, 2, 4
			, 2, 3, 7, 4

			, 4, 5, 1, 5
			, 1, 0, 4, 5
		};


		std::vector<double> data;
		for (int i = 0; i < 12; i++)
		{
			pushVertex(data, BOX_VERTICES[INDICES[i][0]]);
			pushVertex(data, BOX_VERTICES[INDICES[i][1]]);
			pushVertex(data, BOX_VERTICES[INDICES[i][2]]);
		}

		std::vector<double> normals;
		for (int i = 0; i < 12; i++)
		{
			pushVertex(normals, NORMALS[INDICES[i][3]]);
			pushVertex(normals, NORMALS[INDICES[i][3]]);
			pushVertex(normals, NORMALS[INDICES[i][3]]);
		}

		return std::make_tuple(static_cast<UInt>((data.size() / 3)), std::move(data), std::move(normals));
	}

	auto buildBoxMesh(const String& name)
	{
		using VAB = Mesh::VAB;


		auto[elements, data, normals] = std::move(box());

		VAB vertexBuffer = VAB(
			elements
			, static_cast<GLsizei>(data.size() + normals.size())
			, nullptr
		);

		int offset = 0;
		vertexBuffer.bind();

		vertexBuffer.setAttribPointer(static_cast<GLuint>(Attributes::VERTEX), 3, GL_DOUBLE, 3 * sizeof(double), (const void*)offset);
		vertexBuffer.subData(offset, data.size() * sizeof(double), data.data());

		offset += data.size() * sizeof(double);

		vertexBuffer.setAttribPointer(static_cast<GLuint>(Attributes::NORMAL), 3, GL_DOUBLE, 3 * sizeof(double), (const void*)offset);
		vertexBuffer.subData(offset, normals.size() * sizeof(double), normals.data());
		

		UInt materialIndex = 0;

		return Mesh(
			std::move(vertexBuffer)
			, UInt(materialIndex)
			, String(name)
		);
	}
}

void BoxBuilder::build(const String& name)
{
	using Meshes = Model::Meshes;
	using Nodes  = Model::Nodes;
	using Transformations = Model::Transformations;
	using Materials = Model::Materials;

	Meshes meshes(1);
	meshes[0] = std::move(buildBoxMesh(name));

	Nodes nodes(1);
	nodes[0] = std::move(buildSingleNode(name));

	Transformations transformations(1);
	transformations[0] = std::move(buildIdentityTransform());

	Materials materials(0);

	mBuilt = std::move(
		Model(
			  std::move(meshes)
			, std::move(nodes)
			, std::move(transformations)
			, std::move(materials)
		)
	);
}

Model&& BoxBuilder::model()
{
	return std::move(mBuilt);
}