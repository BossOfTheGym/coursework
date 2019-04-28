#include "Actions.h"


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


//Impuls
Impuls::Impuls(
	Satellite* chaser
	, const Vec3& impuls
)
	: ActionBase()
	, mChaser(chaser)
	, mImpuls(impuls)
{}


void Impuls::update(const Time& t, const Time& dt)
{
	if (mState != State::FINISHED && mChaser != nullptr)
	{
		mChaser->mPhysics->mVelocity += mImpuls;
	}

	mState = State::FINISHED;
}


//Wait
Wait::Wait(const Time& wait)
	: ActionBase()
	, mWait(wait)
{}

void Wait::update(const Time& t, const Time& dt)
{
	if (mState != State::FINISHED)
	{
		if (mWait > dt)
		{
			mWait -= dt;
		}
		else
		{
			mState = State::FINISHED;
		}
	}
}