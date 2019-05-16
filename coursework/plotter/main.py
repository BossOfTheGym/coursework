from Rendezvous import circular
from Orbit import Orbit
from ClohessyWilthire import *
from Integrator import classic4, RKE
from utils import *

import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d



def getFloat64():
	
	return np.float64(input())

def getOrbitalParameters():
	print('Inclination        : ', end='')
	i = getFloat64()
	print('Right ascension    : ', end='')
	ra = getFloat64()
	print('Argument of perigee: ', end='')
	ap = getFloat64()
	print('True anomaly       : ', end='')
	ta = getFloat64()
	print('Eccentricity       : ', end='')
	e = getFloat64()
	print('Specific energy    : ', end='')
	h = getFloat64()

	return i, ra, ap, ta, e, h

def getParameters():
	print('Enter mu: ', end='')
	mu = getFloat64()

	print('Enter target orbital params')
	i, ra, ap, ta, e, h = getOrbitalParameters()
	orbitTarget = Orbit()
	orbitTarget.setOrbitFromParams(mu, h, e, np.radians(i), np.radians(ra), np.radians(ap), np.radians(ta))

	print('Enter chaser orbital params')
	i, ra, ap, ta, e, h = getOrbitalParameters()
	orbitChaser = Orbit()
	orbitChaser.setOrbitFromParams(mu, h, e, np.radians(i), np.radians(ra), np.radians(ap), np.radians(ta))

	print('Enter time(hours): ', end='')
	t = getFloat64() * 3600.0

	return mu, orbitTarget, orbitChaser, t



def getCWTrajectory(dr0, dv0, t, n, steps):
	step = t / steps

	dr0 = np.reshape(dr0, (3,))
	dv0 = np.reshape(dv0, (3,))

	x = np.zeros((steps + 1,), np.float64)
	y = np.zeros((steps + 1,), np.float64)
	z = np.zeros((steps + 1,), np.float64)
	for i in range(steps + 1):
		tn = step * i

		r, v = rv(dr0, dv0, tn, n)
		x[i] = r[0]
		y[i] = r[1]
		z[i] = r[2]

	return x, y, z


def packStateVec(r, v):

	return np.float64((r[0], r[1], r[2], v[0], v[1], v[2]))

def unpackStateVec(sv):

	return sv[0 : 3], sv[3 : 6]

def r_rel(r_t, v_t, r_c, v_c):
	h = cross(r_t, v_t)

	i = r_t / length(r_t)
	k = h / length(h)
	j = cross(k, i)

	q = np.float64( ((i[0], i[1], i[2]), (j[0], j[1], j[2]), (k[0], k[1], k[2])) )

	return q @ (r_c - r_t)

def integrateRelativeMotion(mu, orbit_t, orbit_c, t, split):
	def force(t, rv):
		nonlocal mu

		r, v = unpackStateVec(rv)

		r = -mu / dot(r, r) * (r / length(r))

		return packStateVec(v, r)

	solver = RKE(classic4())

	r_t, v_t = orbit_t.r, orbit_t.v
	r_c, v_c = orbit_c.r, orbit_c.v
	sv_c = packStateVec(r_c, v_c)
	sv_t = packStateVec(r_t, v_t)

	n = (int(t) + 1) * split
	step = t / n

	x = np.zeros((n + 1,), np.float64)
	y = np.zeros((n + 1,), np.float64)
	z = np.zeros((n + 1,), np.float64)

	x[0], y[0], z[0] = r_rel(r_t, v_t, r_c, v_c)
	for i in range(n):
		tn = step * i
		
		sv_t = solver.solve(6, force, tn, sv_t, step)
		sv_c = solver.solve(6, force, tn, sv_c, step)

		r_t, v_t = unpackStateVec(sv_t)
		r_c, v_c = unpackStateVec(sv_c)

		x[i + 1], y[i + 1], z[i + 1] = r_rel(r_t, v_t, r_c, v_c)

	return x, y, z


def compress(x, y, z, stride):
	n = ((len(x) // stride) * stride) // stride + 1

	x_new = np.zeros((n + 1,), np.float64)
	y_new = np.zeros((n + 1,), np.float64)
	z_new = np.zeros((n + 1,), np.float64)

	j = 0
	i = 0
	while i < len(x):
		x_new[j] = x[i]
		y_new[j] = y[i]
		z_new[j] = z[i]

		i += stride
		j += 1 
	x_new[-1] = x[-1]
	y_new[-1] = y[-1]
	z_new[-1] = z[-1]

	return x_new, y_new, z_new


def rendezvous(mu, orbitTarget, orbitChaser, t):
	dr0, dv0, dvNew0, dvNew1, dvFirst, dvSecond = circular(orbitTarget, orbitChaser, t)
	orbitChaser.setOrbitFromStateVec(mu, orbitChaser.r, orbitChaser.v + dvFirst)

	# get model % integrated positions
	cw_x, cw_y, cw_z = getCWTrajectory(dr0, dvNew0, t, orbitTarget.n, int(t) * 2)
	x_rel, y_rel, z_rel = integrateRelativeMotion(mu, orbitTarget, orbitChaser, t, 1000)

	x_rel, y_rel, z_rel	= compress(x_rel, y_rel, z_rel, 2000)

	# test print
	print('dr(0)', x_rel[0] , y_rel[0] , z_rel[0], sqrt(x_rel[0]**2 + y_rel[0]**2 + z_rel[0]**2))
	print('dr(t)', x_rel[-1], y_rel[-1], z_rel[-1], sqrt(x_rel[-1]**2 + y_rel[-1]**2 + z_rel[-1]**2))

	# plot54r5
	fig = plt.figure()
	ax = fig.add_subplot(1, 1, 1, projection='3d')
	ax.set_xlabel('\u03B4x')
	ax.set_ylabel('\u03B4y')
	ax.set_zlabel('\u03B4z')
	ax.plot(xs=cw_x, ys=cw_y, zs=cw_z, zdir='z')
	ax.plot(xs=x_rel, ys=y_rel, zs=z_rel, zdir='z')
	plt.show()


if __name__ == '__main__':
	mu, orbitTarget, orbitChaser, t = getParameters()


	# test
	r, v = orbitTarget.r, orbitTarget.v
	orbitTarget.setOrbitFromStateVec(mu, r, v)	

	r, v = orbitChaser.r, orbitChaser.v
	orbitChaser.setOrbitFromStateVec(mu, r, v)


	rendezvous(mu, orbitTarget, orbitChaser, t)
	
