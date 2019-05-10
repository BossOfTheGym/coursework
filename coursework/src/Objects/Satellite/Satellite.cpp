#include "Satellite.h"


Satellite::Satellite(
	IComponent* parent
	, const GraphicsComponentShared&   graphics
	, const PhysicsComponentShared&    physics
	, const SatelliteComponentShared&  satellite
	, const OrbitComponentShared&      orbit
	, const NameComponentShared&       name
)
	: IObject(parent)
	, mGraphics(graphics)
	, mPhysics(physics)
	, mSatellite(satellite)
	, mOrbit(orbit)
	, mName(name)
{}


void Satellite::update(const Time& t, const Time& dt)
{
	if (mOrbit)
	{
		mOrbit->updateOrbit();		
	}
}
