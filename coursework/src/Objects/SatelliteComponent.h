#pragma once

#include "IObject.h"


struct SatelliteComponent : public IComponent
{
public:
	SatelliteComponent(IComponent* parent = nullptr, const Vec3& color = Vec3(1.0f));

	SatelliteComponent(const SatelliteComponent& comp) = default;

	SatelliteComponent(SatelliteComponent&& comp) = default;


	~SatelliteComponent() = default;


	SatelliteComponent& operator = (const SatelliteComponent& comp) = default;

	SatelliteComponent& operator = (SatelliteComponent&& comp) = default;


public:
	virtual const Type& componentType() const override;


public:
	//TODO : behaviour

	Vec3 mColor;
};


//aliases
using SatelliteComponentShared = std::shared_ptr<SatelliteComponent>;
using SatelliteComponentWeak = std::weak_ptr<SatelliteComponent>;