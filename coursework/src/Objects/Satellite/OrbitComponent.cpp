#include "OrbitComponent.h"


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

OrbitComponent::R_V OrbitComponent::orbitState(const Time& dt) const
{
	auto planet    = mPlanetPhys.lock();
	auto satellite = mSatellitePhys.lock();
	if (!planet || !satellite)
	{
		return {Vec3{0.0}, Vec3{0.0}};
	}



	return {Vec3(0.0), Vec3(0.0)};
}

void OrbitComponent::updateOrbit()
{
	using glm::cross;
	using glm::dot;
	using glm::length;

	// i : (1, 0, 0)
	// j : (0, 1, 0)
	// k : (0, 0, 1)
	auto planet    = mPlanetPhys.lock();
	auto satellite = mSatellitePhys.lock();
	if (!planet || !satellite)
	{
		return;
	}

	//algorithm
	mMu = G * planet->mMass;

	//1.
	auto& r0 = satellite->mPosition;
	auto& v0 = satellite->mVelocity;
	auto& center = planet->mPosition;


	auto rv = r0 - center;
	auto r = length(rv);

	//2.
	auto vv = v0;
	auto v = length(v0);
	mH = dot(vv, vv) - 2 * mMu / r;

	//3.
	auto vr = dot(rv / r, vv);

	//4, 5
	mCv = cross(rv, vv);//
	mC  = length(mCv);	
	mP  = mC * mC / mMu;

	//6.
	mI = acos(mCv.z / mC);

	//7, 8
	mNv = cross(Vec3{0.0, 0.0, 1.0}, mCv);
	auto N = length(mNv);

	//9.
	mRA = acos(mNv.x / N);
	if(mNv.y < 0.0f)
	{
		mRA = PI_2 - mRA;
	}	

	//10, 11
	mEv = cross(vv, mCv) / mMu - rv / r;
	mE  = length(mEv);

	//12.
	mAP = acos(dot(mNv / N, mEv / mE));
	if (mEv.z < 0.0f)
	{
		mAP = PI_2 - mAP;
	}

	//13.
	mTA = acos(dot(mEv / mE, rv / r));
	if(vr < 0.0f)
	{
		mTA = PI_2 - mTA;
	}	

	updateSpecificParams();
}

void OrbitComponent::updateSpecificParams()
{
	if (mE < 1.0) // process elliptic only
	{
		mA  = mP / (1.0 - mE * mE);
		mEA = 2 * atan(sqrt((1.0 - mE) / (1.0 + mE)) * tan(mTA / 2));
		if (mTA > PI)
		{
			mEA = PI_2 + mEA;
		}

		auto nInv = pow(mA, 1.5) / sqrt(mMu);

		mT  = nInv * (mEA - mE * sin(mEA));
		mOP = nInv * PI_2;
	}
	else
	{
		mA = 0.0;
		mT = 0.0;
		mOP = 0.0;
		mEA = 0.0;
	}
}