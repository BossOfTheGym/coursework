import numpy as np
from math import *


class Tableau:
	def __init__(self, order, aMat, bVec, cVec):
		self.order = order
		self.aMat = np.copy(aMat)
		self.bVec = np.copy(bVec)
		self.cVec = np.copy(cVec)

def classic4():
	aMat = np.float64(
		(
			  (0.0, 0.0, 0.0, 0.0)
			, (0.5, 0.0, 0.0, 0.0)
			, (0.0, 0.5, 0.0, 0.0)
			, (0.0, 0.0, 1.0, 0.0)
		)
	)

	bVec = np.float64((1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0))

	cVec = np.float64((0.0, 0.5, 0.5, 1.0))

	return Tableau(4, aMat, bVec, cVec)

class RKE:

	def __init__(self, tableau):

		self.tableau = tableau


	def solve(self, order, func, t0, u0, h):
		u0 = np.reshape(u0, (order, ))

		aMat, bVec, cVec = self.tableau.aMat, self.tableau.bVec, self.tableau.cVec


		kVecs = np.zeros((self.tableau.order, order), np.float64)
		for i in range(self.tableau.order):
			for j in range(i):
				kVecs[i] += kVecs[j] * aMat[i][j]

			kVecs[i] = np.reshape(func(t0 + h * cVec[i], u0 + h * kVecs[i]), (order,))


		sum = np.zeros((order, ), np.float64)
		for i in range(self.tableau.order):
			sum += bVec[i] * kVecs[i]


		return u0 + h * sum
