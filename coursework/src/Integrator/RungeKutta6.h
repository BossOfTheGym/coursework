#pragma once

#include "Integrator.h"

#include <Numerics/Ivp/RungeKutta.h>


class RungeKuttaImplicit6 : public IntegratorBase
{
public:
	using Tableau = Num::Ivp::ButcherTableau<double, 3>;
	using Solver  = Num::Ivp::RungeKuttaImplicit<6, double, StateVec, Tableau>;


public:
	RungeKuttaImplicit6(
		  double eps = 1e-15
		, int iterationsLimit = 10
		, Tableau&& tableau = Num::Ivp::Methods<double>::gaussLegendre6()
		, const String& name = "Implicit Runge-Kutta 6"
	);


	RungeKuttaImplicit6(const RungeKuttaImplicit6& rk6) = default;
	RungeKuttaImplicit6(RungeKuttaImplicit6&& rk6)      = default;

	RungeKuttaImplicit6& operator = (const RungeKuttaImplicit6& rk6) = default;
	RungeKuttaImplicit6& operator = (RungeKuttaImplicit6&& rk6)      = default;

	virtual ~RungeKuttaImplicit6() = default;



public:
	virtual void updatePhysics(PlanetShared& planet, SatelliteShared& satellite, const Time& t, const Time& dt) override;


private:
	Solver  mSolver;
};


//aliases
using RungeKuttaImplicit6Shared = std::shared_ptr<RungeKuttaImplicit6>;
using RungeKuttaImplicit6Weak   = std::weak_ptr<RungeKuttaImplicit6>;