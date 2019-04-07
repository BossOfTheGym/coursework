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

	, mOT(0.0f)
	, mEA(0.0f)
	, mT(0.0f)
{}

const IComponent::Type& OrbitComponent::componentType() const
{
	static const Type type = "Orbit";

	return type;
}

void OrbitComponent::update(float t, float dt)
{
	updateOrbit();
}

void OrbitComponent::updateOrbit()
{
	using glm::cross;
	using glm::dot;
	using glm::length;

	// i : (1, 0, 0)
	// j : (0, 1, 0)
	// k : (0, 0, 1)
	auto planet = mPlanetPhys.lock();
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
	auto cv = cross(rv, vv);
	auto c = length(cv);
	mC  = c;
	mCv = cv;
	mP  = dot(cv, cv) / mMu;

	//6.
	auto i = acos(cv.z / c);
	mI = i;

	//7, 8
	auto Nv = cross(Vec3{0.0f, 0.0f, 1.0f}, cv);
	auto N = length(Nv);

	//9.
	auto lan = acos(Nv.x / N);
	if(Nv.y < 0.0f)
	{
		lan = PI_2 - lan;
	}
	mRA = lan;

	//10, 11
	auto ev = cross(vv, cv) / mMu - rv / r;
	auto e  = length(ev);
	mE = e;

	//12.
	auto ap = acos(dot(Nv / N, ev / e));
	if (ev.z < 0.0f)
	{
		ap = PI_2 - ap;
	}
	mAP = ap;

	//13.
	auto ta = acos(dot(ev / e, rv / r));
	if(vr < 0.0f)
	{
		ta = PI_2 - ta;
	}
	mTA = ta;

	updateSpecificParams();
}

void OrbitComponent::updateSpecificParams()
{
	if (mE < 1.0f) // process elliptic only
	{
		mA  = mP / (1.0f - mE * mE);
		mEA = 2.0f * atan(sqrt((1.0f - mE) / (1.0f + mE)) * tan(mTA / 2));

		auto n = pow(mA, 3.0f / 2.0f) / sqrt(mMu);

		mT  = n * (mEA - mE * sin(mEA));
		mOT = n * PI_2;
	}
	else
	{
		mA = 0.0f;
		mT = 0.0f;
		mOT = 0.0f;
		mEA = 0.0f;
	}
}