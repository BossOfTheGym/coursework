#pragma once


#include "IObject.h"

#include "SatelliteComponent.h"


struct Satellite : public IObject
{
public:
	Satellite() = default;

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
};


using SatelliteShared = std::shared_ptr<Satellite>;
using SatelliteWeak = std::weak_ptr<Satellite>;