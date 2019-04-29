#include "Action.h"


//Base
ActionBase::ActionBase()
	: mState(State::INITIALIZING)
{}

void ActionBase::update(const Time& t, const Time& dt)
{
	mState = State::FINISHED;
}

ActionBase::State ActionBase::state() const
{
	return mState;
}