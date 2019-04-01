#include "Planet.h"


void Planet::update(float t, float dt)
{

}

const IComponent::Type& Planet::componentType() const
{
	static const Type type = "Planet";

	return type;
}