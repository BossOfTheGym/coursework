import numpy as np

from ClohessyWilthire import *
from utils import *


def circular(orbitTarget, orbitChaser, t):
	# returns 

	# mean motion
	n = orbitTarget.n
	
	# state parameters
	rChaser = orbitChaser.r
	rTarget = orbitTarget.r

	vChaser = orbitChaser.v
	vTarget = orbitTarget.v

	# specific energy
	hv = orbitTarget.hv

	# angular velocity
	omega = hv / dot(rTarget, rTarget)

	# frame orths
	i = rTarget / length(rTarget)
	k = hv / length(hv)
	j = cross(k, i)

	# transformation matrix
	q  = np.float64( ((i[0], i[1], i[2]), (j[0], j[1], j[2]), (k[0], k[1], k[2])) )
	qi = transpose(q)

	# relative position and velocity
	dr = rChaser - rTarget
	dv = vChaser - vTarget - cross(omega, dr)
	
	dr0 = q @ dr
	dv0 = q @ dv

	# computed impulses
	dvNew0 = -(inverse(phi_rv(t, n)) @ phi_rr(t, n)) @ dr0
	dvNew1 = phi_vr(t, n) @ dr0 + phi_vv(t, n) @ dvNew0

	dvFirstBefore = (dvNew0 - dv0)
	dvFirst  = qi @ dvFirstBefore
	
	dvSecondBefore = -dvNew1
	dvSecond = qi @ dvSecondBefore

	return (dr0, dv0, dvNew0, dvNew1, dvFirst, dvSecond)
