#pragma once

#include "../Method.h"


#include "SimpleActions.h"
#include "ActionVariant.h"



class Circular : public Method
{
public:
	enum class State : int
	{		  
		  EXECUTING    = 1
		, FINISHED     = 0
	};

	using Action  = ActionVariant<Impuls, Wait>;
	using Actions = std::vector<Action>;

	static const int DEFAULT_ACTIONS_CAPACITY = 10;
	

public:
	Circular(
		  const SatelliteWeak& target = SatelliteShared(nullptr)
		, const SatelliteWeak& chaser = SatelliteShared(nullptr)
	);

	Circular(const Circular& c) = delete;
	Circular(Circular&& c)      = default;

	virtual ~Circular() = default;

	Circular& operator = (const Circular& c) = delete;
	Circular& operator = (Circular&& c)      = default;


public:
	virtual void update(const Time& t, const Time& dt) override;


	virtual void start() override;

	virtual void stop() override;

	virtual bool finished() const override;


private:
	State   mState;
	Actions mActions;
};
