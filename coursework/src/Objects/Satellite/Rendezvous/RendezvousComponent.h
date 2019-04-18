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
	enum class Method : int
	{
		THREE_IMPULS
		, LAMBERT_PROBLEM
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
	void start(Method method);

	void reset();


	bool finished() const;

	const SatelliteWeak& target() const;


public:
	Satellite*    mChaser; // from IComponent* mParent
	SatelliteWeak mTarget;
	Actions       mActions;
};


//aliases
using RendezvousComponentShared = std::shared_ptr<RendezvousComponent>;
using RendezvousComponentWeak   = std::weak_ptr<RendezvousComponent>;
