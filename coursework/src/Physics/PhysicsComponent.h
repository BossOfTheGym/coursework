#pragma once


#include <Common.h>

#include <Component/IComponent.h>



struct PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(
		IComponent* parent = nullptr
		, const Mat4& mat = Mat4(1.0f)
		, const Vec3& position = Vec3()
		, const Vec3& velocity = Vec3()
		, const Vec3& angularMomentum = Vec3()
		, double mass = 1.0f
	);

	PhysicsComponent(const PhysicsComponent& comp) = default;
	PhysicsComponent(PhysicsComponent&& comp)      = default;

	virtual ~PhysicsComponent() = default;

	PhysicsComponent& operator = (const PhysicsComponent& comp) = default;
	PhysicsComponent& operator = (PhysicsComponent&& comp)      = default;


public:
	Mat4 mMat;
	Vec3 mPosition;
	Vec3 mVelocity;
	Vec3 mAngularMomentum;

	double mMass;
};


//aliases
using PhysicsComponentShared = std::shared_ptr<PhysicsComponent>;
using PhysicsComponentWeak = std::weak_ptr<PhysicsComponent>;