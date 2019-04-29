#include "Circular.h"


Circular::Circular(const SatelliteWeak& target, const SatelliteWeak& chaser)
	: Method(target, chaser)
	, mState(State::FINISHED)
	, mActions()
{
	mActions.reserve(DEFAULT_ACTIONS_CAPACITY);
}


void Circular::update(const Time& t, const Time& dt)
{
	if (mState == State::EXECUTING)
	{
		//necessary checks
		
		while(true)
		{
			if (mActions.empty())
			{
				mState = State::FINISHED;
				break;
			}

			auto& action = mActions.back();

			action.update(t, dt);
			if (action.state() == ActionBase::State::FINISHED)
			{
				mActions.pop_back();
			}
			else
			{
				break;
			}
		}
	}
}

void Circular::start()
{
	mState = State::EXECUTING;

	mActions.clear();
	mActions.push_back(Impuls(mChaser, Vec3(-1000.0, 0.0, -1000.0)));
	mActions.push_back(Wait(mTime));
	mActions.push_back(Impuls(mChaser, Vec3(1000.0, 100.0, 1000.0)));
}

void Circular::stop()
{
	mState = State::FINISHED;
	mActions.clear();
}
