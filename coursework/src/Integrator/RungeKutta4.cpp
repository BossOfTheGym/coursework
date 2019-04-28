#include "RungeKutta4.h"


RungeKuttaExplicit4::RungeKuttaExplicit4(Tableau&& tableau, const String& name)
	: IntegratorBase(name)
	, mSolver(std::move(tableau))
{}


void RungeKuttaExplicit4::updatePhysics(PlanetShared& planet, SatelliteShared& satellite, const Time& t, const Time& dt)
{
	using glm::dot;
	using glm::length;


	//parameters
	Mat4& mat = satellite->mPhysics->mMat;
	Vec3& r   = satellite->mPhysics->mPosition;
	Vec3& v   = satellite->mPhysics->mVelocity;

	Vec3& r0 = planet->mPhysics->mPosition;

	double mu = G * planet->mPhysics->mMass;

	Vec3 dr = r - r0;


	//force
	auto force = [&] (double t, const StateVec& vec) -> StateVec
	{
		Vec3 rv = unpackRadius(vec);
		Vec3 vv = unpackVelocity(vec);

		double r = length(rv);

		Vec3 newV = -mu / (r * r) * (rv / r);

		return packStateVec(vv, newV);
	};


	//integrate
	StateVec curr = packStateVec(dr, v);
	StateVec next = mSolver.solve(
		std::move(force)
		, t.asFloat()
		, curr
		, dt.asFloat()
	).second;


	//update
	r = unpackRadius(next) + r0;
	v = unpackVelocity(next);

	mat[3] = Vec4(r, 1.0);
}