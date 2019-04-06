#pragma once 


#include "../IObject.h"

#include <Physics/PhysicsComponent.h>
#include <Graphics/GraphicsComponent.h>

struct Planet : public IObject
{
public:
	Planet() = default;

	Planet(const Planet& planet) = default;

	Planet(Planet&& planet) = default;
	

	~Planet() = default;


	Planet& operator = (const Planet& planet) = default;

	Planet& operator = (Planet&& planet) = default;


public:
	virtual void update(float t, float dt) override;

	virtual const Type& componentType() const override;


public:
	GraphicsComponentShared mGraphics;
	PhysicsComponentShared mPhysics;
};


using PlanetShared = std::shared_ptr<Planet>;
using PlanetWeak = std::weak_ptr<Planet>;