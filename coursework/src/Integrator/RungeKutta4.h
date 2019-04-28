#pragma once

#include "Integrator.h"

#include <Numerics/Ivp/RungeKutta.h>



class RungeKuttaExplicit4 : public IntegratorBase
{
public:
	using Tableau = Num::Ivp::ButcherTableau<double, 4>;
	using Solver  = Num::Ivp::RungeKuttaExplicit<6, double, StateVec, Tableau>;


public:
	RungeKuttaExplicit4(
		  Tableau&& tableau = Num::Ivp::Methods<double>::classic4()
		, const String& name = "Explicit Runge-Kutta 4"
	);


	RungeKuttaExplicit4(const RungeKuttaExplicit4& rk4) = default;
	RungeKuttaExplicit4(RungeKuttaExplicit4&& rk4)      = default;

	RungeKuttaExplicit4& operator = (const RungeKuttaExplicit4& rk4) = default;
	RungeKuttaExplicit4& operator = (RungeKuttaExplicit4&& rk4)      = default;

	virtual ~RungeKuttaExplicit4() = default;


public:
	virtual void updatePhysics(PlanetShared& planet, SatelliteShared& satellite, const Time& t, const Time& dt) override;


private:
	Solver  mSolver;
};


//aliases
using RungeKuttaExplicit4Shared = std::shared_ptr<RungeKuttaExplicit4>;
using RungeKuttaExplicit4Weak   = std::weak_ptr<RungeKuttaExplicit4>;