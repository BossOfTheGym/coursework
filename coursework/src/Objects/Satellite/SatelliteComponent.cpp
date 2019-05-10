#include "SatelliteComponent.h"


SatelliteComponent::SatelliteComponent(
	IComponent* parent
	, const Vec3& color
)
	: IComponent(parent)
	, mColor(color)
{}
