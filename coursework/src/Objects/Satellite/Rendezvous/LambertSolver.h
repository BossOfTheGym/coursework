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
	//x = -h * s^2, s - universal var, theta - transfer angle
	struct X_S_T
	{
		double x;
		double s;
		double theta;
	};

	X_S_T solve(
		  const Vec3& rv1
		, double t1
		, const Vec3& rv2
		, double t2
		, double mu
		, double x0
		, double eps = 1e-15f
		, int limit = 10
	);
}
