#pragma once

#include <Common.h>

#include "../IObject.h"

#include <Physics/PhysicsComponent.h>


struct OrbitComponent : public IComponent
{
public:
	struct R_V
	{
		Vec3 radius;
		Vec3 velocity;
	};

public:
	OrbitComponent(
		  IComponent* parent
		, const PhysicsComponentWeak& planet
		, const PhysicsComponentWeak& satellite
	);

	OrbitComponent(const OrbitComponent& comp) = default;
	OrbitComponent(OrbitComponent&& comp)      = default;

	virtual ~OrbitComponent() = default;

	OrbitComponent& operator = (const OrbitComponent& comp) = default;
	OrbitComponent& operator = (OrbitComponent&& comp)      = default;


public:
	virtual const Type& componentType() const override;
	

public:
	R_V orbitStateAngle(double theta) const;

	R_V orbitStateTime(const Time& dt) const;


	void updateOrbit();


private:
	void updateSpecificParams();

	
public:
	PhysicsComponentWeak mSatellitePhys;
	PhysicsComponentWeak mPlanetPhys;

	//main
	double mC;   // specific Angular Momentum(length)
	double mI;   // Inclination
	double mRA;  // right ascension of the Ascending Node
	double mE;   // Eccentricity
	double mAP;  // Argument of Periapsis
	double mTA;  // True Anomaly

	//misc
	Vec3 mCv;   // specific Angular Momentum
	Vec3 mEv;   // apsis line
	Vec3 mNv;   // node line

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