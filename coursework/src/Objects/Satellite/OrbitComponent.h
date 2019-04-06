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

	OrbitComponent(OrbitComponent&& comp) = default;


	virtual ~OrbitComponent() = default;


	OrbitComponent& operator = (const OrbitComponent& comp) = default;

	OrbitComponent& operator = (OrbitComponent&& comp) = default;



public:
	virtual const Type& componentType() const override;
	

public:
	void updateOrbit();

private:
	void updateSpecificParams();

	
public:
	//refs
	PhysicsComponentWeak mPlanetPhys;
	PhysicsComponentWeak mSatellitePhys;

	//main
	float mC;   // specific Angular Momentum(c lowercase)
	float mI;   // Inclination(i lowercase)
	float mRA;  // right ascension of the Ascending Node(omega uppercase)
	float mE;   // Eccentricity(e lowercase)
	float mAP;  // Argument of Periapsis(omega lowercase)
	float mTA;  // True Anomaly(theta lowercase)

	//misc
	float mP;   // semi-latus rectum(p lowercase)
	float mMu;  // mu param

	//specific
	float mOT;  // Orbit Period
	float mA;   // apoapsis
	float mEA;  // Eccentric Anomaly(e uppercase)
	float mT;   // time since periapsis	
};


//aliases
using OrbitComponentShared = std::shared_ptr<OrbitComponent>;
using OrbitComponentWeak   = std::weak_ptr<OrbitComponent>;