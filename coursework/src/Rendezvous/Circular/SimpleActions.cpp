#include "SimpleActions.h"


//Impuls
Impuls::Impuls(
	  const SatelliteWeak& chaser
	, const Vec3& impuls
)
	: ActionBase()
	, mChaser(chaser)
	, mImpuls(impuls)
{}


void Impuls::update(const Time& t, const Time& dt)
{
	if (mState != State::FINISHED)
	{
		if (auto ptr = mChaser.lock())
		{
			ptr->mPhysics->mVelocity += mImpuls;
		}
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