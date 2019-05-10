#include "Planet.h"


Planet::Planet(IComponent* parent, const NameComponentShared& name)
	: IObject(parent)
	, mName(name)
{}


void Planet::update(const Time& t, const Time& dt)
{
	
}
