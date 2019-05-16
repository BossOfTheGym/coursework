import numpy as np

from math import sqrt


def inverse(mat):
	
	return np.linalg.inv(mat)


def transpose(mat):

	return np.transpose(mat)


def dot(a, b):

	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def cross(a, b):
	cp = np.empty((3,), np.float64)

	cp[0] = a[1] * b[2] - a[2] * b[1]
	cp[1] = a[2] * b[0] - a[0] * b[2]
	cp[2] = a[0] * b[1] - a[1] * b[0]

	return cp


def length(vec):
	
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2])