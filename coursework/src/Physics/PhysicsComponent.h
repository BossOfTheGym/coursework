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
	);

	PhysicsComponent(const PhysicsComponent& comp) = default;

	PhysicsComponent(PhysicsComponent&& comp) = default;


	~PhysicsComponent() = default;


	PhysicsComponent& operator = (const PhysicsComponent& comp) = default;

	PhysicsComponent& operator = (PhysicsComponent&& comp) = default;


	virtual const Type& componentType() const override;


public:
	Mat4 mMat;
	Vec3 mPosition;
	Vec3 mVelocity;
	Vec3 mAngularMomentum;

	float mass;
};


//aliases
using PhysicsComponentShared = std::shared_ptr<PhysicsComponent>;
using PhysicsComponentWeak = std::weak_ptr<PhysicsComponent>;