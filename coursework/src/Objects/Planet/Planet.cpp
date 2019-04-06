#include "Planet.h"


Planet::Planet(IComponent* parent, const NameComponentShared& name)
	: IObject(parent)
	, mName(name)
{}


void Planet::update(float t, float dt)
{

}

const IComponent::Type& Planet::componentType() const
{
	static const Type type = "Planet";

	return type;
}