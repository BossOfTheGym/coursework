#include "OrbitComponent.h"

#include <limits>

#include <Numerics/Equ/Neuton.h>


OrbitComponent::OrbitComponent(
	IComponent* parent
	, const PhysicsComponentWeak& planet
	, const PhysicsComponentWeak& satellite
)
	: IComponent(parent)

	, mPlanetPhys(planet)
	, mSatellitePhys(satellite)

	, mC(0.0f)
	, mI(0.0f)
	, mRA(0.0f)
	, mE(0.0f)
	, mAP(0.0f)
	, mTA(0.0f)

	, mP(0.0f)
	, mMu(0.0f)

	, mOP(0.0f)
	, mEA(0.0f)
	, mT(0.0f)
{
	updateOrbit();
}

const IComponent::Type& OrbitComponent::componentType() const
{
	static const Type type = "Orbit";

	return type;
}

//TODO
OrbitComponent::R_V OrbitComponent::orbitStateAngle(double theta) const
{
	//TODO

	return {};
}

OrbitComponent::R_V OrbitComponent::orbitStateTime(const Time& dt) const
{
	//TODO

	return {};
}


void OrbitComponent::updateOrbit()
{
	using glm::cross;
	using glm::dot;
	using glm::length;


	auto planet    = mPlanetPhys.lock();
	auto satellite = mSatellitePhys.lock();
	if (!planet || !satellite)
	{
		return;
	}

	//algorithm
	mMu = G * planet->mMass;

	//1.
	Vec3& r0 = satellite->mPosition;
	Vec3& v0 = satellite->mVelocity;
	Vec3& center = planet->mPosition;


	Vec3  rv = r0 - center;
	double r = length(rv);

	//2.
	Vec3  vv = v0;
	double v = length(v0);
	mH = dot(vv, vv) - 2.0 * mMu / r;

	//3.
	double vr = dot(rv / r, vv);

	//4, 5
	mCv = cross(rv, vv);//
	mC  = length(mCv);	
	mP  = mC * mC / mMu;

	//6.
	mI = acos(mCv.z / mC);

	//7, 8
	mNv = cross(Vec3(0.0, 0.0, 1.0), mCv);
	double N = length(mNv);
	
	//9.
	mRA = acos(mNv.x / N);
	if(mNv.y <= std::numeric_limits<double>::epsilon())
	{
		mRA = PI_2 - mRA;
	}	

	//10, 11
	mEv = cross(vv, mCv) / mMu - rv / r;
	mE  = length(mEv);

	//12.
	mAP = acos(dot(mNv / N, mEv / mE));
	if (mEv.z <= std::numeric_limits<double>::epsilon())
	{
		mAP = PI_2 - mAP;
	}

	//13.
	mTA = acos(dot(rv / r, mEv / mE));
	if(vr <= std::numeric_limits<double>::epsilon())
	{
		mTA = PI_2 - mTA;
	}	

	updateSpecificParams();
}

void OrbitComponent::updateSpecificParams()
{
	if (mE < 1.0) //elliptic
	{
		mA  = mP / (1.0 - mE * mE);
		mEA = 2.0 * atan(sqrt((1.0 - mE) / (1.0 + mE)) * tan(mTA / 2.0));
		if (mTA > PI)
		{
			mEA = PI_2 + mEA;
		}

		auto nInv = pow(mA, 1.5) / sqrt(mMu);

		mT  = nInv * (mEA - mE * sin(mEA));
		mOP = nInv * PI_2;
	}
	else if (mE > 1.0) // hyperbolic
	{
		mA  = mP / (mE * mE - 1.0);
		mEA = 2 * atanh(sqrt((mE - 1.0) / (mE + 1.0)) * tanh(mTA / 2));

		auto nInv = pow(mA, 1.5) / sqrt(mMu);

		mT  = nInv * (mE * sinh(mEA) - mEA); //something strange happens here
		mOP = std::numeric_limits<double>::infinity();
	}
	else
	{
		mA = 0.0;
		mT = 0.0;
		mOP = 0.0;
		mEA = 0.0;
	}
}