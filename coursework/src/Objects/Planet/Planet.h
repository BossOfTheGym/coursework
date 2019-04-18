#pragma once 


#include "../IObject.h"

#include <Physics/PhysicsComponent.h>
#include <Graphics/GraphicsComponent.h>
#include <Objects/NameComponent.h>


struct Planet : public IObject
{
public:
	Planet(IComponent* parent = nullptr, const NameComponentShared& = nullptr);

	Planet(const Planet& planet) = default;

	Planet(Planet&& planet) = default;
	

	~Planet() = default;


	Planet& operator = (const Planet& planet) = default;

	Planet& operator = (Planet&& planet) = default;


public:
	virtual void update(const Time& t, const Time& dt) override;

	virtual const Type& componentType() const override;


public:
	GraphicsComponentShared mGraphics;
	PhysicsComponentShared mPhysics;
	NameComponentShared mName;
};


using PlanetShared = std::shared_ptr<Planet>;
using PlanetWeak = std::weak_ptr<Planet>;