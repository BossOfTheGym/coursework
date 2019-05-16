import numpy as np

from math import cos, sin


def phi_rr(t, n):
	nt = n * t
	cos_nt = cos(nt)
	sin_nt = sin(nt)
	
	return np.float64(
		(
			  (4.0 - 3.0 * cos_nt ,  0.0,  0.0   )
			, (6.0 * (sin_nt - nt),  1.0,  0.0   )
			, (0.0                ,  0.0,  cos_nt) 
		)
	)

def phi_rv(t, n):
	nt = n * t
	cos_nt = cos(nt)
	sin_nt = sin(nt)

	ni  = 1.0 / n;
	ni2 = 2.0 * ni;

	return np.float64(
		(
			  (ni * sin_nt         ,  ni2 * (1.0 - cos_nt)          ,  0.0        )
			, (ni2 * (cos_nt - 1.0),  ni * (4.0 * sin_nt - 3.0 * nt),  0.0        )
			, (0.0                 ,  0.0                           ,  ni * sin_nt)
		)
	)
	

def phi_vr(t, n):
	nt = n * t
	cos_nt = cos(nt)
	sin_nt = sin(nt)

	return np.float64(
		(
			  (3.0 * n * sin_nt        ,  0.0,  0.0        )
			, (6.0 * n * (cos_nt - 1.0),  0.0,  0.0        )
			, (0.0                     ,  0.0,  -n * sin_nt)
		)
	)

def phi_vv(t, n):
	nt = n * t
	cos_nt = cos(nt)
	sin_nt = sin(nt)

	return np.float64(
		(
			  (cos_nt       ,  2.0 * sin_nt      ,  0.0   )
			, (-2.0 * sin_nt,  4.0 * cos_nt - 3.0,  0.0   )
			, (0.0          ,  0.0               ,  cos_nt)
		)
	)


def rv(dr0, dv0, t, n):
	dr = np.matmul(phi_rr(t, n), dr0) + np.matmul(phi_rv(t, n), dv0)
	dv = np.matmul(phi_vr(t, n), dr0) + np.matmul(phi_vv(t, n), dv0)

	return dr, dv