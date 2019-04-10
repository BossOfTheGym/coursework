#pragma once


#include <Common.h>

#include <Physics/PhysicsComponent.h>
#include <Graphics/GraphicsComponent.h>
#include <Objects/Objects.h>


//object factories
IObjectShared createSatellite(
	const ModelShared& model
	, float mass
	, const Vec3& color = Vec3(1.0f)
	, const Mat4& mat = Mat4(1.0f)
	, const Vec3& pos = Vec3()
	, const Vec3& vel = Vec3()
	, const String& name = ""
	, const PhysicsComponentWeak& planet = PhysicsComponentShared(nullptr)
);


IObjectShared createPlanet(
	const ModelShared& model
	, float mass
	, const Mat4& mat = Mat4(1.0f)
	, const Vec3& pos = Vec3()
	, const Vec3& vel = Vec3()
	, const Vec3& angularMomentum = Vec3()
	, const String& name = ""
);