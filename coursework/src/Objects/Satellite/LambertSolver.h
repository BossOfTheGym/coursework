#pragma once

#include <Common.h>

namespace Stumpff
{
	double c0(double x);

	double c1(double x);

	double c2(double x);

	double c3(double x);

	double c4(double x);

	double c5(double x);

	double c6(double x);
}

namespace Lambert
{
	//case 1:
	//if revolutions count = 0 then velocity1 is for hyperbolic trajectory
	//and velocity2 is for elliptical trajectory
	//case 2:
	//both velocities are for elliptical trajectory
	struct Solution
	{
		Vec3 vel1;
		Vec3 vel2;
	};

	Solution solve(
		  const Vec3& rv1
		, double t1
		, const Vec3& rv2
		, double t2
		, double mu
		, unsigned revolutions
		, double eps = 1e-15f
		, int limit = 10
	);
}
