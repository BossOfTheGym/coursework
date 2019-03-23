#include "PlanetModel.h"


//statics
const UInt PlanetMesh::MATERIAL = 0;


//constructors & destructor
PlanetMesh::PlanetMesh()
	: mName("")
	, mVertexBuffer()
{}

PlanetMesh::PlanetMesh(UInt split, const String& name)
	: mName(name)
	, mVertexBuffer()
{
	initMesh(split);
}

PlanetMesh::PlanetMesh(PlanetMesh&& mesh) : PlanetMesh()
{
	*this = std::move(mesh);
}


PlanetMesh::~PlanetMesh()
{}


//operators
PlanetMesh& PlanetMesh::operator = (PlanetMesh&& mesh)
{
	if (this != &mesh)
	{
		std::swap(mName, mesh.mName);
		std::swap(mVertexBuffer, mesh.mVertexBuffer);
	}

	return *this;
}


//IObject
const String& PlanetMesh::toString() const
{
	return mName;
}


//IMesh
const IMesh::VAB& PlanetMesh::vab() const
{
	return mVertexBuffer;
}

const UInt& PlanetMesh::material() const
{
	return MATERIAL;
}


//init mesh
void PlanetMesh::initMesh(UInt split)
{
	auto[elements, dome] = std::move(geoDome(split));

	mVertexBuffer = std::move(
		VertexArrayBuffer(
			elements
			, static_cast<GLsizei>(dome.size())
			, dome.data()
		)
	);

	mVertexBuffer.bindArray();
	mVertexBuffer.setAttribPointer(VERTEX, 3, GL_FLOAT, 3 * sizeof(float), 0);
}



namespace
{
	void pushVertex(std::vector<float>& data, const Vec3& vertex)
	{
		data.push_back(vertex.x);
		data.push_back(vertex.y);
		data.push_back(vertex.z);
	}

	void pushSplitedTriangles(
		  std::vector<float>& data 
		, const Vec3& vertex0
		, const Vec3& vertex1
		, const Vec3& vertex2
		, UInt split)
	{
		if (split > 0)
		{
			Vec3 vertex01 = (vertex0 + vertex1) / 2.0f;
			Vec3 vertex12 = (vertex1 + vertex2) / 2.0f;
			Vec3 vertex20 = (vertex2 + vertex0) / 2.0f;

			pushSplitedTriangles(data, vertex0, vertex01, vertex20, split - 1);

			pushSplitedTriangles(data, vertex01, vertex1, vertex12, split - 1);

			pushSplitedTriangles(data, vertex12, vertex2, vertex20, split - 1);

			pushSplitedTriangles(data, vertex01, vertex12, vertex20, split - 1);
		}
		else
		{
			pushVertex(data, vertex0);
			pushVertex(data, vertex1);
			pushVertex(data, vertex2);
		}
	}
}

DomeData geoDome(UInt split)
{
	const int PENTAGON_SIDES = 5;

	const float HALF_ANGLE = PI / PENTAGON_SIDES;
	const float ANGLE      = 2 * HALF_ANGLE;
	const float SIN_HALF   = sin(HALF_ANGLE);

	const float ICOSAHEDRON_SIDE = sqrt(4 * pow(SIN_HALF, 2) - 1.0f) / SIN_HALF;
	const float PYRAMID_HEIGHT   = pow(ICOSAHEDRON_SIDE, 2) / 2;
	const float RADIUS           = ICOSAHEDRON_SIDE / (2 * SIN_HALF);
	const float HEIGHT_ANGLE = atan((1.0f - PYRAMID_HEIGHT) / RADIUS);

	const float A   = ICOSAHEDRON_SIDE;
	const float H   = PYRAMID_HEIGHT;
	const float R   = RADIUS;
	const float PHI = HEIGHT_ANGLE;

	const int N = PENTAGON_SIDES;


	//init icosahedron verteces
	glm::vec3 top    = glm::vec3(0.0f, 0.0f, +1.0f);
	glm::vec3 bottom = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3    topRing[N];
	glm::vec3 bottomRing[N];


	float angle;
	for (int i = 0; i < N; i++) 
	{
		angle = ANGLE * i;
		topRing[i] = glm::vec3(R * cos(angle), R * sin(angle), 1.0f - H);

		angle = ANGLE * i + HALF_ANGLE;
		bottomRing[i] = glm::vec3(R * cos(angle), R * sin(angle), H - 1.0f);	
	}


	//assemble triangles
	std::vector<float> data;
	for (int curr = 0; curr < N; curr++)
	{
		int prev = (curr + N - 1) % N;
		int next = (curr + N + 1) % N;

		//top + topRing[i] + topRing[i + 1]
		pushSplitedTriangles(data, top, topRing[curr], topRing[next], split);

		//bottom + bottomRing[i] + bottomRing[i + 1]
		pushSplitedTriangles(data, bottom, bottomRing[next], bottomRing[curr], split);

		//bottomRing[i] + topRing[i] + topRing[i + 1]
		pushSplitedTriangles(data, bottomRing[curr], topRing[next], topRing[curr], split);;

		//topRing[i] + bottomRing[i] + bottomRing[i - 1]
		pushSplitedTriangles(data, topRing[curr], bottomRing[prev], bottomRing[curr], split);
	}

	return std::make_tuple(static_cast<UInt>(data.size() / 3), data);
}