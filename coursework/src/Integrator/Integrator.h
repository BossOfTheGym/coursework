#pragma once

#include <Common.h>
#include <Objects/Objects.h>

#include <Numerics/Arg/ArgN.h>



//Utils
using StateVec = Num::Arg::VecN<double, 6>;
using Matrix   = Num::Arg::MatNxM<double, 6, 6>;


StateVec packStateVec(const Vec3& radius, const Vec3& velocity);

Vec3 unpackRadius(const StateVec& stateVec);

Vec3 unpackVelocity(const StateVec& stateVec);


struct Parameters
{

};



class IntegratorBase
{
public:
	IntegratorBase(const String& name = "Integrator");

	IntegratorBase(const IntegratorBase& integrator) = default;
	IntegratorBase(IntegratorBase&& integrator)      = default;

	IntegratorBase& operator = (const IntegratorBase& integrator) = default;
	IntegratorBase& operator = (IntegratorBase&& integrator)      = default;

	virtual ~IntegratorBase() = default;


public:
	virtual void updatePhysics(PlanetShared& planet, SatelliteShared& satellite, const Time& t, const Time& dt) = 0;


public:
	const String& name() const;


protected:
	String mName;
};


//aliases
using IntegratorShared = std::shared_ptr<IntegratorBase>;
using IntegratowWeak   = std::weak_ptr<IntegratorBase>;