#pragma once

#include <Common.h>


namespace ClohessyWitshire
{
	struct DrDv
	{
		Vec3 dr;
		Vec3 dv;
	};


	//n - mean motion
	//t - time


	Mat3 phi_rr(double t, double n);

	Mat3 phi_rv(double t, double n);

	Mat3 phi_vr(double t, double n);

	Mat3 phi_vv(double t, double n);


	DrDv dr_dv(const Vec3& dr0, const Vec3& dv0, double t, double n);
}
