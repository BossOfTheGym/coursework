#include "Satellite.h"


Satellite::Satellite(
	IComponent* parent
	, const Time& time
	, const GraphicsComponentShared&  graphics
	, const PhysicsComponentShared&   physics
	, const SatelliteComponentShared& satellite
	, const OrbitComponentShared&     orbit
	, const NameComponentShared&      name
)
	: IObject(parent, time)
	, mGraphics(graphics)
	, mPhysics(physics)
	, mSatellite(satellite)
	, mOrbit(orbit)
	, mName(name)
{}


void Satellite::update(const Time& t)
{
	if (mOrbit)
	{
		mOrbit->updateOrbit();
	}
	mTime = t;
}

const IComponent::Type& Satellite::componentType() const
{
	static const Type type = "Satellite";

	return type;
}