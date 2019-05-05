#include "ClohessyWitshire.h"

#pragma once

#include <Common.h>


namespace ClohessyWitshire
{
	//n - mean motion
	//t - time

	Mat3 phi_rr(double t, double n)
	{
		double nt = n * t;
		double cos_nt = cos(nt);
		double sin_nt = sin(nt);

		return Mat3(
			  (4.0 - 3.0 * cos_nt), (6.0 * (sin_nt - nt)), (0.0)
			, (0.0)               , (1.0)                , (0.0)
			, (0.0)               , (0.0)                , (cos_nt)
		);
	}

	Mat3 phi_rv(double t, double n)
	{
		double nt = n * t;
		double cos_nt = cos(nt);
		double sin_nt = sin(nt);

		double ni = 1.0 / n;
		double ni2 = 2.0 * ni;

		return Mat3(
			  (ni * sin_nt)         , (ni2 * (cos_nt - 1.0))          , (0.0)
			, (ni2 * (1.0 - cos_nt)), (ni * (4.0 * sin_nt - 3.0 * nt)), (0.0)
			, (0.0)                 , (0.0)                           , (ni * sin_nt)
		);
	}

	Mat3 phi_vr(double t, double n)
	{
		double nt = n * t;
		double cos_nt = cos(nt);
		double sin_nt = sin(nt);

		return Mat3(
			  (3.0 * n * sin_nt), (6.0 * n * (cos_nt - 1.0)), (0.0)
			, (0.0)             , (0.0)                     , (0.0)
			, (0.0)             , (0.0)                     , (-n * sin_nt)
		);
	}

	Mat3 phi_vv(double t, double n)
	{
		double nt = n * t;
		double cos_nt = cos(nt);
		double sin_nt = sin(nt);

		return Mat3(
			  (cos_nt)      , (-2.0 * sin_nt)     , (0.0)
			, (2.0 * sin_nt), (4.0 * cos_nt - 3.0), (0.0)
			, (0.0)         , (0.0)               , (cos_nt)
		);
	}


	DrDv dr_dv(const Vec3& dr0, const Vec3& dv0, double t, double n)
	{
		return {
			  (phi_rr(t, n) * dr0 + phi_rv(t, n) * dv0)
			, (phi_vr(t, n) * dr0 + phi_vv(t, n) * dv0)
		};
	}
}
