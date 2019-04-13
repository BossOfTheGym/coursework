#pragma once

#include <Common.h>

#include <Entity/ITimeVarying.h>
#include <Physics/PhysicsComponent.h>

#include "../../IObject.h"
#include "../Satellite.h"


//Satellite forward decl
struct Satellite;

struct RendezvousComponent : public IComponent, public ITimeVarying
{
public:
	using Commands = std::vector<Command>;


public:
	RendezvousComponent(
		IComponent* parent = nullptr
		, const SatelliteWeak& target = SatelliteShared(nullptr)
	);

	RendezvousComponent(const RendezvousComponent& comp) = default;

	RendezvousComponent(RendezvousComponent&& comp) = default;


	virtual ~RendezvousComponent() = default;


	RendezvousComponent& operator = (const RendezvousComponent& comp) = default;

	RendezvousComponent& operator = (RendezvousComponent&& comp) = default;

	
public:
	virtual const Type& componentType() const override;

	virtual void update(float t, float dt) override;


public:
	void start();

	void stop();


	bool finished() const;

	SatelliteWeak target() const;


public:
	SatelliteWeak mTarget;
	Commands mCommandStack;
};



