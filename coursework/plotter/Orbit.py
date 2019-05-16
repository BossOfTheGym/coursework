import numpy as np

from math import *
from utils import *



class Orbit:

	def __init__(self):
		# main
		self.h  = None # specific energy(specific angular momentum)
		self.i  = None # inclination
		self.ra = None # right ascension
		self.ap = None # argument of periapsis
		self.ta = None # true anomaly
		self.e  = None # eccentricity

		# misc
		self.hv = None # specific energy(specific angular momentum)
		self.ev = None # apsis line
		self.nv = None # node line

		self.en = None  # energy
		self.p  = None  # semi-latus rectum
		
		# specific
		self.n  = None # mean motion
		self.op = None # orbital period
		self.a  = None # apoapsis
		self.ea = None # eccentric anomaly
		self.t  = None # time

		# state parameters
		self.r = None
		self.v = None


	def setOrbitFromStateVec(self, mu, r, v):
		self.r = r
		self.v = v

		# 1.
		rl = length(r)

		# 2.
		vl = length(v)
		self.en = dot(v, v) - 2.0 * mu / rl

		# 3.
		vr = dot(r / rl, v)

		# 4, 5
		self.hv = cross(r, v)
		self.h = length(self.hv)
		self.p = self.h**2 / mu	

		# 6.
		self.i = acos(self.hv[2] / self.h)
		
		# 7, 8
		self.nv = cross(np.float64((0.0, 0.0, 1.0)), self.hv)
		N = length(self.nv)

		# 9.
		self.ra = acos(self.nv[0] / N)
		if self.nv[1] <= 0.0:
			self.ra = 2 * pi - self.ra

		# 10, 11
		self.ev = cross(v, self.hv) / mu - r / rl
		self.e = length(self.ev)

		uev = np.zeros((3,), np.float64) if self.e <= 0.0 else self.ev / self.e

		# 12.
		self.ap = acos(dot(self.nv / N, uev))
		if self.ap <= 0.0:
			self.ap = 2 * pi - self.ap

		# 13.
		self.ta = acos(dot(r / rl, uev))
		if vr <= 0.0:
			self.ta = 2 * pi - self.ta

		self.__updateSpecificParameters(mu)


	def setOrbitFromParams(self, mu, h, e, i, ra, ap, ta):
		cost = cos(ta);
		sint = sin(ta);

		coso = cos(ap);
		sino = sin(ap);
		cosO = cos(ra);
		sinO = sin(ra);
		cosi = cos(i);
		sini = sin(i);

		rx = (h**2 / mu / (1.0 + e * cost)) * np.float64((cost, sint, 0.0))
		vx = (mu / h) * np.float64((-sint, e + cost, 0.0))

		Qxx = np.float64(
			(
				  (
					  cosO * coso - sinO * sino * cosi
					, -cosO * sino - sinO * cosi * coso
					,  sinO * sini
				  )
				, (
					  sinO * coso + cosO * cosi * sino
					, -sinO * sino + cosO * cosi * coso
					, -cosO * sini
				  )
				, (
					sini * sino
					, sini * coso
					, cosi
				  )
			)
		)

		self.h  = h
		self.e  = e
		self.i  = i
		self.ra = ra
		self.ap = ap
		self.ta = ta

		self.r = np.matmul(Qxx, rx)
		self.v = np.matmul(Qxx, vx)

		self.en = dot(self.v, self.v) / 2 - mu / length(self.r)

		self.p = h * h / mu

		self.hv = cross(self.r, self.v)
		self.ev = cross((0.0, 0.0, 1.0), self.hv)
		self.nv = cross(self.v, self.hv) / mu - self.r / length(self.r)


		self.__updateSpecificParameters(mu)


	def __updateSpecificParameters(self, mu):
		e  = self.e
		p  = self.p
		ta = self.ta

		if e < 1.0:
			self.a  = p / (1.0 - e * e)
			self.ea = 2.0 * atan(sqrt((1.0 - e) / (1.0 + e))) * tan(ta / 2.0)
			self.ea = self.ea if ta < pi else 2.0 * pi + self.ea

			self.n = sqrt(mu) / self.a**1.5
			
			self.t  = 1.0 / self.n * (self.ea - e * sin(self.ea))
			self.op = 1.0 / self.n * pi * 2.0

		elif self.e > 1.0:
			self.a  = p / (e * e - 1.0)
			self.ea = 2.0 * atanh(sqrt((e - 1.0) / (e + 1.0))) * tanh(ta / 2.0)

			self.n = sqrt(mu) / self.a**1.5
			
			self.t  = 1.0 / self.n * (e * sinh(self.ea) - self.ea)
			self.op = inf

		else:
			self.a  = 0.0
			self.t  = 0.0
			self.op = 0.0
			self.ea = 0.0
