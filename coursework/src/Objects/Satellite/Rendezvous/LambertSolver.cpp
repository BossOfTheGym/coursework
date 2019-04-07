#include "LambertSolver.h"


#include <Numerics/Equ/Neuton.h>


namespace
{
	//x > 0

	float c0(float x)
	{
		return cos(sqrt(x));
	}

	float c1(float x)
	{
		auto sq = sqrt(x);

		return sin(sq) / sq;
	}

	float c2(float x)
	{
		return (1.0f - cos(sqrt(x))) / x;
	}

	float c3(float x)
	{
		auto sq = sqrt(x);

		return (sq - sin(sq)) / (x * sq);
	}

	float c4(float x)
	{
		return (1.0f / 2 - c2(x)) / x;
	}

	float c5(float x)
	{
		return (1.0f / 6 - c3(x)) / x;
	}

	float c6(float x)
	{
		return (1.0f / 24 - c4(x)) / x;
	}


	//float F(float x, float rho, float sigma)
	//{
	//	auto c1Val = c1(x);
	//	auto c2Val = c2(x);
	//	auto c3Val = c3(x);
	//
	//	auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));
	//
	//	return c3(x) / pow(c2(x), 1.5f) * pow(u, 3.0f) + rho * u - sigma;
	//}

	//float Fx(float x, float rho)
	//{
	//	auto c1Val = c1(x);
	//	auto c2Val = c2(x);
	//	auto c3Val = c3(x);
	//	auto c4Val = c4(x);
	//	auto c5Val = c5(x);
	//	auto c6Val = c6(x);
	//
	//	auto c2Pow = pow(c2Val, 1.5f);
	//
	//	auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));
	//
	//	return
	//		+(c3Val * c3Val - c5Val + 4 * c6Val) / (4 * c2Pow) * pow(u, 3)
	//		+(3 * c3Val / c2Pow * u * u + rho) * rho * sqrt(c2Val) / (8 * u);
	//}

}


float solveLambertProblem(
	const Vec3& rv1
	, float t1
	, const Vec3& rv2
	, float t2
	, float mu
	, float x0
	, float eps
	, int limit
)
{
	using glm::dot;
	using glm::length;

	auto r1 = length(rv1);
	auto r2 = length(rv2);

	//cos(2a) = 2 * cos(a)^2 - 1
	auto cosTransfer = dot(rv1 / r1, rv2 / r2);
	auto cosHalf = sqrt((cosTransfer + 1) / 2);

	auto rho   = sqrt(2 * r1 * r2) / (r1 + r2) * cosHalf;
	auto sigma = sqrt(mu) / pow(r1 + r2, 1.5f) * (t2 - t1);

	//func
	auto F = [&] (float x) -> float
	{
		auto c1Val = c1(x);
		auto c2Val = c2(x);
		auto c3Val = c3(x);

		auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));

		return c3(x) / pow(c2(x), 1.5f) * pow(u, 3.0f) + rho * u - sigma;
	};

	//deriv
	auto Fx = [&] (float x) -> float
	{
		auto c1Val = c1(x);
		auto c2Val = c2(x);
		auto c3Val = c3(x);
		auto c4Val = c4(x);
		auto c5Val = c5(x);
		auto c6Val = c6(x);

		auto c2Pow = pow(c2Val, 1.5f);

		auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));

		return
			+(c3Val * c3Val - c5Val + 4 * c6Val) / (4 * c2Pow) * pow(u, 3)
			+(3 * c3Val / c2Pow * u * u + rho) * rho * sqrt(c2Val) / (8 * u);
	};

	return Num::Equ::NeutonScalar<float>(limit, eps).solve(F, Fx, x0).first;
}
