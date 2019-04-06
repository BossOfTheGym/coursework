#include "SatelliteComponent.h"


SatelliteComponent::SatelliteComponent(
	IComponent* parent
	, const Vec3& color
)
	: IComponent(parent)
	, mColor(color)
{}


const IComponent::Type& SatelliteComponent::componentType() const
{
	static const Type type = "Satellite";

	return type;
}
