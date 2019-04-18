#pragma once

#include <Common.h>

#include <Numerics/Arg/ArgN.h>
#include <Physics/PhysicsComponent.h>


//radius(r) + velocity(v)
class StateVec : public Num::Arg::VecN<double, 6>
{
public:
	StateVec();

	StateVec(const Vec3& radius, const Vec3& velocity);

	StateVec(double r0, double r1, double r2, double v0, double v1, double v2);


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