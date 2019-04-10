#include "Factories.h"


//object factories
IObjectShared createSatellite(
	const ModelShared& model
	, float mass
	, const Vec3& color
	, const Mat4& mat
	, const Vec3& pos
	, const Vec3& vel
	, const String& name
	, const PhysicsComponentWeak& planet
)
{
	SatelliteShared satellite = std::make_shared<Satellite>();


	satellite->mPhysics  = PhysicsComponentShared(
		new PhysicsComponent(satellite.get(), mat, pos, vel, Vec3(), mass)
	);

	satellite->mGraphics = GraphicsComponentShared(
		new GraphicsComponent(
			satellite.get()
			, model
			, satellite->mPhysics
		)
	);

	satellite->mSatellite = SatelliteComponentShared(
		new SatelliteComponent(nullptr, color)
	);

	satellite->mName = NameComponentShared(
		new NameComponent(satellite.get(), name)
	);

	satellite->mOrbit = OrbitComponentShared(
		new OrbitComponent(
			satellite.get()
			, planet
			, satellite->mPhysics
		)
	);


	return satellite;
}

IObjectShared createPlanet(
	const ModelShared& model
	, float mass
	, const Mat4& mat
	, const Vec3& pos
	, const Vec3& vel
	, const Vec3& angularMomentum
	, const String& name
)
{
	PlanetShared planet = std::make_shared<Planet>();


	planet->mPhysics  = PhysicsComponentShared(
		new PhysicsComponent(nullptr, mat, pos, vel, angularMomentum, mass)
	);

	planet->mGraphics = GraphicsComponentShared(
		new GraphicsComponent(
			nullptr
			, model
			, planet->mPhysics
		)
	);


	return planet;
}