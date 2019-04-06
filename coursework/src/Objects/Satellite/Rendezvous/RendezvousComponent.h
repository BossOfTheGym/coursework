#pragma once

#include <Common.h>

#include <Entity/ITimeVarying.h>
#include <Physics/PhysicsComponent.h>

#include "../../IObject.h"
#include "../Satellite.h"

struct Satellite;



//state machine
//always has default state : Floating
struct RendezvousControl : public IComponent
{
public:

public:
	RendezvousControl(
		IComponent* parent = nullptr
		, const SatelliteWeak& target = SatelliteShared(nullptr)
	);

	RendezvousControl(const RendezvousControl& comp) = default;

	RendezvousControl(RendezvousControl&& comp) = default;


	virtual ~RendezvousControl() = default;


	RendezvousControl& operator = (const RendezvousControl& comp) = default;

	RendezvousControl& operator = (RendezvousControl&& comp) = default;

	
public:
	virtual const Type& componentType() const override;


public:
	void pushCommand();

	void popCommand();

	void peekCommand();

	void clear();


public:
	SatelliteWeak mTarget;
};
