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
	OrbitComponent(OrbitComponent&& comp) = default;

	virtual ~OrbitComponent() = default;

	OrbitComponent& operator = (const OrbitComponent& comp) = default;
	OrbitComponent& operator = (OrbitComponent&& comp) = default;


public:
	R_V orbitStateAngle(double theta) const;

	R_V orbitStateTime(const Time& dt) const;


	void updateOrbit();

	void setOrbit(double h, double e, double i, double Omega, double omega, double thetha);


	//get
	double angularMomentum() const
	{
		return mH;
	}

	double inclination() const
	{
		return mI;
	}

	double rightAscension() const
	{
		return mRA;
	}

	double eccentricity() const
	{
		return mE;
	}

	double argumentOfPeriapsis() const
	{
		return mAP;
	}

	double trueAnomaly() const
	{
		return mTA;
	}


	Vec3 angularMomentumV() const
	{
		return mHv;
	}

	Vec3 apsisLineV() const
	{
		return mEv;
	}

	Vec3 nodeLineV() const
	{
		return mNv;
	}


	double energy() const
	{
		return mEn;
	}

	double semi_latusRectum() const
	{
		return mP;
	}

	double mu() const
	{
		return mMu;
	}

	double meanMotion() const
	{
		return mN;
	}



	double period() const
	{
		return mOP;
	}

	double apoapsis() const
	{
		return mA;
	}

	double eccentricAnomaly() const
	{
		return mEA;
	}

	double time() const
	{
		return mT;
	}


private:
	void updateSpecificParams();


private:
	PhysicsComponentWeak mSatellitePhys;
	PhysicsComponentWeak mPlanetPhys;

	//main
	double mH;   // specific Angular Momentum(length)
	double mI;   // Inclination
	double mRA;  // right ascension of the Ascending Node
	double mE;   // Eccentricity
	double mAP;  // Argument of Periapsis
	double mTA;  // True Anomaly

				//misc
	Vec3 mHv;   // specific Angular Momentum
	Vec3 mEv;   // apsis line
	Vec3 mNv;   // node line

	double mEn;  // energy
	double mP;   // semi-latus rectum
	double mMu;  // mu param

				 //specific
	double mN;   // mean motion
	double mOP;  // Orbit Period
	double mA;   // apoapsis
	double mEA;  // Eccentric Anomaly
	double mT;   // time since periapsis	
};


//aliases
using OrbitComponentShared = std::shared_ptr<OrbitComponent>;
using OrbitComponentWeak = std::weak_ptr<OrbitComponent>;


//#pragma once
//
//#include <Common.h>
//
//#include "../IObject.h"
//
//#include <Physics/PhysicsComponent.h>
//
//
//struct OrbitComponent : public IComponent
//{
//public:
//	struct R_V
//	{
//		Vec3 radius;
//		Vec3 velocity;
//	};
//
//public:
//	OrbitComponent(IComponent* parent);
//
//	OrbitComponent(const OrbitComponent& comp) = default;
//	OrbitComponent(OrbitComponent&& comp)      = default;
//
//	virtual ~OrbitComponent() = default;
//
//	OrbitComponent& operator = (const OrbitComponent& comp) = default;
//	OrbitComponent& operator = (OrbitComponent&& comp)      = default;
//
//
//public:
//	R_V orbitStateAngle(double theta) const;
//
//	R_V orbitStateTime(const Time& dt) const;
//
//
//	void setOrbitFromStateVec(double mu, const Vec3& center, const Vec3& r, const Vec3& v);
//
//	void setOrbitFromParams(double mu, double h, double e, double i, double Omega, double omega, double thetha);
//
//
//	//get
//	double angularMomentum() const
//	{
//		return m_h;
//	}
//
//	double inclination() const
//	{
//		return m_i;
//	}
//
//	double rightAscension() const
//	{
//		return m_ra;
//	}
//
//	double eccentricity() const
//	{
//		return m_e;
//	}
//
//	double argumentOfPeriapsis() const
//	{
//		return m_ap;
//	}
//
//	double trueAnomaly() const
//	{
//		return m_ta;
//	}
//
//
//	Vec3 angularMomentumV() const
//	{
//		return m_hv;
//	}
//
//	Vec3 apsisLineV() const
//	{
//		return m_ev;
//	}
//
//	Vec3 nodeLineV() const
//	{
//		return m_nv;
//	}
//
//
//	double energy() const
//	{
//		return m_en;
//	}
//
//	double semi_latusRectum() const
//	{
//		return m_p;
//	}
//
//	double meanMotion() const
//	{
//		return m_n;
//	}
//
//
//			
//	double period() const
//	{
//		return m_op;
//	}
//
//	double apoapsis() const
//	{
//		return m_a;
//	}
//
//	double eccentricAnomaly() const
//	{
//		return m_ea;
//	}
//
//	double time() const
//	{
//		return m_t;
//	}
//
//
//private:
//	void updateSpecificParams(double mu);
//
//
//private:
//	//main
//	double m_h;   // specific Angular Momentum(length)
//	double m_i;   // Inclination
//	double m_ra;  // right ascension of the Ascending Node
//	double m_e;   // Eccentricity
//	double m_ap;  // Argument of Periapsis
//	double m_ta;  // True Anomaly
//
//	//misc
//	Vec3 m_hv;   // specific Angular Momentum
//	Vec3 m_ev;   // apsis line
//	Vec3 m_nv;   // node line
//
//	double m_en;   // energy integral
//	double m_p;   // semi-latus rectum
//
//	//specific
//	double m_n;   // mean motion
//	double m_op;  // Orbit Period
//	double m_a;   // apoapsis
//	double m_ea;  // Eccentric Anomaly
//	double m_t;   // time since periapsis	
//
//	//state vec
//	Vec3 m_r;
//	Vec3 m_v;
//};
//
//
////aliases
//using OrbitComponentShared = std::shared_ptr<OrbitComponent>;
//using OrbitComponentWeak   = std::weak_ptr<OrbitComponent>;