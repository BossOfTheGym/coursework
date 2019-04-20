#include "LambertSolver.h"


#include <Numerics/Equ/Neuton.h>


namespace Stumpff
{
	//x > 0

	double c0(double x)
	{
		return cos(sqrt(x));
	}

	double c1(double x)
	{
		auto sq = sqrt(x);

		return sin(sq) / sq;
	}

	double c2(double x)
	{
		return (1.0f - cos(sqrt(x))) / x;
	}

	double c3(double x)
	{
		auto sq = sqrt(x);

		return (sq - sin(sq)) / (x * sq);
	}

	double c4(double x)
	{
		return (1.0f / 2 - c2(x)) / x;
	}

	double c5(double x)
	{
		return (1.0f / 6 - c3(x)) / x;
	}

	double c6(double x)
	{
		return (1.0f / 24 - c4(x)) / x;
	}


	//double F(double x, double rho, double sigma)
	//{
	//	auto c1Val = c1(x);
	//	auto c2Val = c2(x);
	//	auto c3Val = c3(x);
	//
	//	auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));
	//
	//	return c3(x) / pow(c2(x), 1.5f) * pow(u, 3.0f) + rho * u - sigma;
	//}

	//double Fx(double x, double rho)
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

namespace Lambert
{
	X_S solve(
		const Vec3& rv1
		, double t1
		, const Vec3& rv2
		, double t2
		, double mu
		, double x0
		, double eps
		, int limit
	)
	{
		using namespace Stumpff;

		using Solver = Num::Equ::NeutonScalar<double>;

		using glm::dot;
		using glm::length;


		auto r1 = length(rv1);
		auto r2 = length(rv2);

		//cos(2a) = 2 * cos(a)^2 - 1
		auto cosTransfer = dot(rv1 / r1, rv2 / r2);
		auto cosHalf = sqrt((cosTransfer + 1) / 2);

		auto rho   = sqrt(2 * r1 * r2) / (r1 + r2) * cosHalf;
		auto sigma = sqrt(mu) / pow(r1 + r2, 1.5) * (t2 - t1);

		//func
		auto F = [&] (double x) -> double
		{
			auto c1Val = c1(x);
			auto c2Val = c2(x);
			auto c3Val = c3(x);

			auto uVal  = sqrt(1.0 - rho * c1Val / sqrt(c2Val));
			auto uVal3 = uVal * uVal * uVal;

			return c3(x) / pow(c2(x), 1.5) * uVal3 + rho * uVal - sigma;
		};

		//deriv
		auto Fx = [&] (double x) -> double
		{
			auto c1Val = c1(x);
			auto c2Val = c2(x);
			auto c3Val = c3(x);
			auto c4Val = c4(x);
			auto c5Val = c5(x);
			auto c6Val = c6(x);

			auto c2Pow = pow(c2Val, 1.5);

			auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));

			return
				+(c3Val * c3Val - c5Val + 4 * c6Val) / (4 * c2Pow) * pow(u, 3)
				+(3 * c3Val / c2Pow * u * u + rho) * rho * sqrt(c2Val) / (8 * u);
		};
		
		auto x = Solver(limit, eps).solve(F, Fx, x0).first;
		auto u = sqrt(1.0 - rho * c1(x) / sqrt(c2(x)));
		auto s = sqrt((r1 + r2) / (mu * c2(x))) * u;

		return {x, s};
	}
}
