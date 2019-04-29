#pragma once


#include <Common.h>

#include <Objects/Satellite/Satellite.h>


class ActionBase
{
public:
	enum class State : int
	{
		INITIALIZING = -1
		, EXECUTING    = +0
		, FINISHED     = +1
	};


protected:
	ActionBase();

	ActionBase(const ActionBase& ab) = default;
	ActionBase(ActionBase&& ab)      = default;

	~ActionBase() = default;

	ActionBase& operator = (const ActionBase& ab) = default;
	ActionBase& operator = (ActionBase&& ab)      = default;


public:
	void update(const Time& t, const Time& dt);

	State state() const;


protected:
	State mState;
};