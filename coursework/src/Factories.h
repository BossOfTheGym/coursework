#pragma once


#include <Common.h>

#include <Physics/PhysicsComponent.h>
#include <Graphics/GraphicsComponent.h>
#include <Objects/Objects.h>


//object factories
SatelliteShared createSatellite(
	const ModelShared& model
	, double mass
	, const Vec3& color = Vec3(1.0f)
	, const Mat4& mat = Mat4(1.0f)
	, const Vec3& pos = Vec3()
	, const Vec3& vel = Vec3()
	, const String& name = ""
	, const PhysicsComponentWeak& planet = PhysicsComponentShared(nullptr)
);


PlanetShared createPlanet(
	const ModelShared& model
	, double mass
	, const Mat4& mat = Mat4(1.0f)
	, const Vec3& pos = Vec3()
	, const Vec3& vel = Vec3()
	, const Vec3& angularMomentum = Vec3()
	, const String& name = ""
);