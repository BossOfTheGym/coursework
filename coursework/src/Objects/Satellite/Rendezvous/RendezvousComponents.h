#pragma once

#include <Common.h>

#include <Entity/ITimeVarying.h>
#include <Physics/PhysicsComponent.h>

#include "../../IObject.h"


#include "State.h"
#include "Floating.h"
#include "CloseProximity.h"
#include "FarProximity.h"
#include "Waiting.h"


//state machine
//always has default state : Floating
struct RendezvousControl : public IComponent, public ITimeVarying
{
public:
	using StateStack = std::vector<StateShared>;


public:
	RendezvousControl(IComponent* parent = nullptr);

	RendezvousControl(const RendezvousControl& comp) = default;

	RendezvousControl(RendezvousControl&& comp) = default;


	virtual ~RendezvousControl() = default;


	RendezvousControl& operator = (const RendezvousControl& comp) = default;

	RendezvousControl& operator = (RendezvousControl&& comp) = default;

	
public:
	virtual const Type& componentType() const override;

	virtual void update(float t, float dt) override;


public:
	const State& peekState();

	void pushState(const StateShared& state);

	void popState();

	void clearStack();



public:
	PhysicsComponentWeak mTarget;

private:
	StateStack mStates;
};
