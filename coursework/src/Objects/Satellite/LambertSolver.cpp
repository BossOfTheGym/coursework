#include "LambertSolver.h"


#include <Numerics/Equ/Neuton.h>


namespace Stumpff
{
	double c0(double x)
	{
		if (x > std::numeric_limits<double>::epsilon())
		{
			return cos(sqrt(x));
		}
		else if (x < std::numeric_limits<double>::epsilon())
		{
			return cosh(sqrt(-x));
		}
		else
		{
			return 1.0;
		}

		return 0.0;
	}

	double c1(double x)
	{
		if (x > std::numeric_limits<double>::epsilon())
		{
			double xSq = sqrt(x);

			return sin(xSq) / xSq;
		}
		else if (x < std::numeric_limits<double>::epsilon())
		{
			double xSq = sqrt(-x);

			return sinh(xSq) / xSq;
		}
		else
		{
			return 1.0;
		}

		return 0.0;
	}

	double c2(double x)
	{
		if (x > std::numeric_limits<double>::epsilon())
		{
			return (1.0f - cos(sqrt(x))) / x;
		}
		else if (x < std::numeric_limits<double>::epsilon())
		{
			return (cosh(sqrt(-x)) - 1.0) / (-x);
		}
		else
		{
			return 1.0 / 2;
		}

		return 0.0;
	}

	double c3(double x)
	{
		if (x > std::numeric_limits<double>::epsilon())
		{
			double sq = sqrt(x);

			return (sq - sin(sq)) / (x * sq);
		}
		else if (x < std::numeric_limits<double>::epsilon())
		{
			double sq = sqrt(-x);

			return (sinh(sq) - sq) / (-x * sq);
		}
		else
		{
			return 1.0 / 6;
		}

		return 0.0;
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
}

namespace Lambert
{
	//initialD  :D
	struct InitialX
	{
		double x0;
		double x1;
	};

	InitialX x_K_revolutions(double rho, double sigma, unsigned k)
	{		
		double um = sqrt(1.0 - sqrt(2.0) * abs(rho));

		double eps1 = pow(PI * k / (sqrt(2.0) * (sigma - rho * um)), 1.0 / 3.0) * um;
		double eps2 = pow(PI * (k + 1) / (2.0 / 3.0 * pow(um, 3.0) + sigma - rho * um), 1.0 / 3.0) * um;

		double z1 = eps1;
		double z2 = PI - eps2;

		return {4 * pow(z1 + PI * k, 2), 4 * pow(z2 + PI * k, 2)};
	}

	double C(double z)
	{
		if (z > std::numeric_limits<double>::epsilon())
		{
			return (1.0 - cos(sqrt(z))) / z;
		}
		else if (z < std::numeric_limits<double>::epsilon())
		{
			return (cosh(sqrt(-z)) - 1.0) / -z;
		}
		else
		{
			return 1.0 / 2;
		}

		return 0.0;
	}

	double S(double z)
	{
		if (z > std::numeric_limits<double>::epsilon())
		{
			double zSq = sqrt(z);
			double zSq3 = z * zSq;

			return (zSq - sin(zSq)) / zSq3;

		}
		else if (z < std::numeric_limits<double>::epsilon())
		{
			double zSq  = sqrt(-z);
			double zSq3 = -z * zSq;

			return (sinh(zSq) - zSq) / zSq3;
		}
		else
		{
			return 1.0 / 6;
		}

		return 0.0;
	}


	Solution solve(
		  const Vec3& rv1  // rad-vec 1
		, double t1        // time1
		, const Vec3& rv2  // rad-vec 2
		, double t2        // time2
		, double mu        // G * M
		, unsigned revolutions  // revolutions on transfer
		, double eps       // eps for solver
		, int limit        // iterations limit
	)
	{
		using namespace Stumpff;

		using Solver = Num::Equ::NeutonScalar<double>;

		using glm::dot;
		using glm::cross;
		using glm::length;



		double r1 = length(rv1);
		double r2 = length(rv2);

		auto transfer = acos(dot(rv1 / r1, rv2 / r2));
		if (cross(rv1, rv2).z < 0.0)
		{
			transfer = PI_2 - transfer;
		}

		double tau = t2 - t1;
		double rho   = sqrt(2 * r1 * r2) / (r1 + r2) * cos(transfer / 2);
		double sigma = sqrt(mu) / pow(r1 + r2, 1.5) * tau;

	
		auto[x1, x2] = x_K_revolutions(rho, sigma, revolutions);

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

			auto c3Val2 = c3Val * c3Val;

			auto c2Pow = pow(c2Val, 1.5);

			auto u = sqrt(1.0f - rho * c1Val / sqrt(c2Val));
			auto u2 = u * u;
			auto u3 = u2 * u;

			return
				+(c3Val2 - c5Val + 4.0 * c6Val) / (4.0 * c2Pow) * u3
				+(3.0 * c3Val / c2Pow * u2 + rho) * rho * sqrt(c2Val) / (8 * u);
		};
		
		//get velocity vector
		auto solution = [&] (double x0) -> Vec3
		{
			auto[x, iter] = Solver(limit, eps).solve(F, Fx, x0);

			double u = sqrt(1.0 - rho * c1(x) / sqrt(c2(x)));
			double s = sqrt((r1 + r2) / (mu * c2(x))) * u;
			double s2 = s * s;
			double s3 = s2 * s;

			double f = 1.0 - mu * s2 * c2(x) / r1;
			double g = tau - mu * s3 * c3(x);

			return 1.0 / g * (rv2 - f * rv1);
		};

		return {solution(x1), solution(x2)};
	}
}
