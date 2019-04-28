#include "Integrator.h"



//Utils
StateVec packStateVec(const Vec3& radius, const Vec3& velocity)
{
	return StateVec(radius[0], radius[1], radius[2], velocity[0], velocity[1], velocity[2]);
}

Vec3 unpackRadius(const StateVec& stateVec)
{
	return Vec3(stateVec[0], stateVec[1], stateVec[2]);
}

Vec3 unpackVelocity(const StateVec& stateVec)
{
	return Vec3(stateVec[3], stateVec[4], stateVec[5]);
}



//Integrator base
IntegratorBase::IntegratorBase(const String& name)
	: mName(name)
{}

const String& IntegratorBase::name() const
{
	return mName;
}