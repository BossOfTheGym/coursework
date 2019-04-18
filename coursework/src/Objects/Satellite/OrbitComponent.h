#pragma once

#include <Common.h>

#include "../IObject.h"

#include <Physics/PhysicsComponent.h>


struct OrbitComponent : public IComponent
{
public:
	OrbitComponent(
		  IComponent* parent = nullptr
		, const PhysicsComponentWeak& planet    = PhysicsComponentShared(nullptr)
		, const PhysicsComponentWeak& satellite = PhysicsComponentShared(nullptr)
	);

	OrbitComponent(const OrbitComponent& comp) = default;
	OrbitComponent(OrbitComponent&& comp)      = default;

	virtual ~OrbitComponent() = default;

	OrbitComponent& operator = (const OrbitComponent& comp) = default;
	OrbitComponent& operator = (OrbitComponent&& comp)      = default;


public:
	virtual const Type& componentType() const override;
	
public:
	

	void updateOrbit();

private:
	void updateSpecificParams();

	
public:
	//cache
	PhysicsComponentWeak mPlanetPhys;
	PhysicsComponentWeak mSatellitePhys;

	//main
	double mC;   // specific Angular Momentum(length)
	double mI;   // Inclination
	double mRA;  // right ascension of the Ascending Node
	double mE;   // Eccentricity
	double mAP;  // Argument of Periapsis
	double mTA;  // True Anomaly

	//misc
	Vec3 mCv;   // specific Angular Momentum
	Vec3 mEv;   // eccentricity vector
	Vec3 mLv;   // lagrange vector

	double mH;   // energy integral
	double mP;   // semi-latus rectum
	double mMu;  // mu param

	//specific
	double mOP;  // Orbit Period
	double mA;   // apoapsis
	double mEA;  // Eccentric Anomaly
	double mT;   // time since periapsis	
};


//aliases
using OrbitComponentShared = std::shared_ptr<OrbitComponent>;
using OrbitComponentWeak   = std::weak_ptr<OrbitComponent>;