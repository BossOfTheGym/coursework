#include "Integrator.h"



//State Vector
StateVec::StateVec()
	: VecN()
{}

StateVec::StateVec(const Vec3& radius, const Vec3& velocity)
{
	setRadius(radius);
	setVelocity(velocity);
}

StateVec::StateVec(float r0, float r1, float r2, float v0, float v1, float v2)
{
	m_vec[0] = r0;
	m_vec[1] = r1;
	m_vec[2] = r2;
	m_vec[3] = v0;
	m_vec[4] = v1;
	m_vec[5] = v2;
}


Vec3 StateVec::r() const
{
	return Vec3(m_vec[0], m_vec[1], m_vec[2]);
}

Vec3 StateVec::v() const
{
	return Vec3(m_vec[3], m_vec[4], m_vec[5]);
}


void StateVec::setRadius(const Vec3& radius)
{
	m_vec[0] = radius[0];
	m_vec[1] = radius[1];
	m_vec[2] = radius[2];
}

void StateVec::setVelocity(const Vec3& velocity)
{
	m_vec[3] = velocity[3];
	m_vec[4] = velocity[4];
	m_vec[5] = velocity[5];
}


//Integrator base