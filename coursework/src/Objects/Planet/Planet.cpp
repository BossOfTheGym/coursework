#include "Planet.h"


Planet::Planet(IComponent* parent, const Time& t, const NameComponentShared& name)
	: IObject(parent, t)
	, mName(name)
{}


void Planet::update(const Time& t)
{
	mTime = t;
}

const IComponent::Type& Planet::componentType() const
{
	static const Type type = "Planet";

	return type;
}