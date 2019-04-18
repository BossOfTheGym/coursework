#include "Satellite.h"


Satellite::Satellite(
	IComponent* parent
	, const GraphicsComponentShared&   graphics
	, const PhysicsComponentShared&    physics
	, const SatelliteComponentShared&  satellite
	, const OrbitComponentShared&      orbit
	, const NameComponentShared&       name
	, const RendezvousComponentShared& rendezvous
)
	: IObject(parent)
	, mGraphics(graphics)
	, mPhysics(physics)
	, mSatellite(satellite)
	, mOrbit(orbit)
	, mName(name)
	, mRendezvous(rendezvous)
{}


void Satellite::update(const Time& t, const Time& dt)
{
	if (mOrbit)
	{
		mOrbit->updateOrbit();		
	}

	if (mRendezvous)
	{
		mRendezvous->update(t, dt);
	}
}

const IComponent::Type& Satellite::componentType() const
{
	static const Type type = "Satellite";

	return type;
}