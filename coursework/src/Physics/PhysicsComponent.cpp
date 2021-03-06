#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(
	IComponent* parent
	, const Mat4& mat
	, const Vec3& position
	, const Vec3& velocity
	, const Vec3& angularMomentum
	, double mass
) 
	: IComponent(parent)
	, mMat(mat)
	, mPosition(position)
	, mVelocity(velocity)
	, mAngularMomentum(angularMomentum)
	, mMass(mass)
{
	mMat[3] = Vec4(mPosition, 1.0f);
}