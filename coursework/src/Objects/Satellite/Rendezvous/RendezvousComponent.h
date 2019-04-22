#pragma once

#include <Common.h>

#include <Physics/PhysicsComponent.h>

#include "../../IObject.h"

#include "Actions.h"


//forward decl
template<class ... Types>
class ActionVariant;

class Impuls;
class Wait;

//forward decl
struct Satellite;
using SatelliteShared = std::shared_ptr<Satellite>;
using SatelliteWeak   = std::weak_ptr<Satellite>;


struct RendezvousComponent : public IComponent, public ITimeVarying
{
public:
	static const size_t DEFAULT_CAPACITY = 10;


public:
	enum class State : int
	{
		  STARTED = 1
		, FINISHED = 0
	};


public:
	using Action  = ActionVariant<Impuls, Wait>;
	using Actions = std::vector<Action>;


public:
	RendezvousComponent(
		IComponent* parent = nullptr
		, const SatelliteWeak& target = SatelliteShared(nullptr)
	);

	RendezvousComponent(const RendezvousComponent& comp) = default;
	RendezvousComponent(RendezvousComponent&& comp)      = default;

	virtual ~RendezvousComponent() = default;

	RendezvousComponent& operator = (const RendezvousComponent& comp) = default;
	RendezvousComponent& operator = (RendezvousComponent&& comp)      = default;

	
public:
	virtual const Type& componentType() const override;

	virtual void update(const Time& t, const Time& dt) override;


public:
	void start();

	void stop();

	template<class ActionType>
	void pushAction(ActionType&& action)
	{
		if (mState == State::FINISHED)
		{
			mActions.push_back(action);
		}
	}

	template<class ActionType>
	void pushAction(const Action& action)
	{
		if (mState == State::FINISHED)
		{
			mActions.push_back(action);
		}
	}

	State state() const;


	void setTarget(const SatelliteWeak& target);

	const SatelliteWeak& target() const;


public:
	Satellite*    mChaser; // from IComponent* mParent
	SatelliteWeak mTarget;
	Actions       mActions;
	State         mState;
};


//aliases
using RendezvousComponentShared = std::shared_ptr<RendezvousComponent>;
using RendezvousComponentWeak   = std::weak_ptr<RendezvousComponent>;
