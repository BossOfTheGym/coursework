#pragma once


#include "../IObject.h"
#include "SatelliteComponent.h"
#include "OrbitComponent.h"

#include <Physics/PhysicsComponent.h>
#include <Graphics/GraphicsComponent.h>
#include <Objects/NameComponent.h>


struct Satellite : public IObject
{
public:
	Satellite(
		IComponent* parent = nullptr
		, const GraphicsComponentShared&  graphics  = nullptr
		, const PhysicsComponentShared&   physics   = nullptr
		, const SatelliteComponentShared& satellite = nullptr
		, const OrbitComponentShared&     orbit     = nullptr
		, const NameComponentShared&      name      = nullptr
	);

	Satellite(const Satellite& sat) = default;

	Satellite(Satellite&& sat) = default;


	~Satellite() = default;


	Satellite& operator = (const Satellite& sat) = default;

	Satellite& operator = (Satellite&& sat) = default;



public:
	virtual void update(float t, float dt) override;
	
	virtual const Type& componentType() const override;

	
public:
	GraphicsComponentShared  mGraphics;
	PhysicsComponentShared   mPhysics;
	SatelliteComponentShared mSatellite;
	OrbitComponentShared     mOrbit;
	NameComponentShared      mName;
};


using SatelliteShared = std::shared_ptr<Satellite>;
using SatelliteWeak = std::weak_ptr<Satellite>;