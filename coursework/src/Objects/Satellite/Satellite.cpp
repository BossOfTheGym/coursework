#include "Satellite.h"


Satellite::Satellite(
	IComponent* parent
	, const GraphicsComponentShared&  graphics
	, const PhysicsComponentShared&   physics
	, const SatelliteComponentShared& satellite
	, const OrbitComponentShared&     orbit
	, const NameComponentShared&      name
)
	: IObject(parent)
	, mGraphics(graphics)
	, mPhysics(physics)
	, mSatellite(satellite)
	, mOrbit(orbit)
	, mName(name)
{}


void Satellite::update(float t, float dt)
{
	if (mOrbit)
	{
		mOrbit->updateOrbit();
	}
}

const IComponent::Type& Satellite::componentType() const
{
	static const Type type = "Satellite";

	return type;
}