#include "Satellite.h"


void Satellite::update(float t, float dt)
{

}

const IComponent::Type& Satellite::componentType() const
{
	static const Type type = "Satellite";

	return type;
}