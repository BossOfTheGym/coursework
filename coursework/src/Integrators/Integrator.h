#pragma once

#include <Common.h>

#include <Numerics/Arg/ArgN.h>
#include <Physics/PhysicsComponent.h>


//radius(r) + velocity(v)
class StateVec : public Num::Arg::VecN<float, 6>
{
public:
	StateVec();

	StateVec(const Vec3& radius, const Vec3& velocity);

	StateVec(float r0, float r1, float r2, float v0, float v1, float v2);


	StateVec(const StateVec& sv) = default;

	StateVec(StateVec&& sv) = default;


	~StateVec() = default;


	StateVec& operator = (const StateVec& sv) = default;

	StateVec& operator = (StateVec&& sv) = default;



	Vec3 r() const;

	Vec3 v() const;


	void setRadius(const Vec3& radius);

	void setVelocity(const Vec3& velocity);
};


class IntegratorBase
{
public:
	virtual void updatePhysics(PhysicsComponentShared planet, PhysicsComponentShared satellite) = 0;
};