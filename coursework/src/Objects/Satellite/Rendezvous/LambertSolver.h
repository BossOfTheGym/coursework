#pragma once

#include <Common.h>

float solveLambertProblem(
	const Vec3& rv1
	, float t1
	, const Vec3& rv2
	, float t2
	, float mu
	, float x0
	, float eps = 1e-10f
	, int limit = 10
);