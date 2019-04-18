#pragma once

#include <Common.h>

double solveLambertProblem(
	const Vec3& rv1
	, double t1
	, const Vec3& rv2
	, double t2
	, double mu
	, double x0
	, double eps = 1e-10f
	, int limit = 10
);