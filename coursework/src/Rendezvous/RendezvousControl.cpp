#include "RendezvousControl.h"


RendezvousControl::RendezvousControl(
	MethodUnique&& method
	, const Time& t
)
	: mMethod(std::move(method))
{
	setTime(t);
}


void RendezvousControl::update(const Time& t, const Time& dt)
{
	if (mMethod)
	{
		mMethod->update(t, dt);
	}
}

void RendezvousControl::start()
{
	if (mMethod)
	{
		mMethod->start();
	}
}

void RendezvousControl::stop()
{
	if (mMethod)
	{
		mMethod->stop();
	}
}


void RendezvousControl::setTarget(const SatelliteWeak& target)
{
	if (mMethod)
	{
		mMethod->setTarget(target);
	}
}

void RendezvousControl::setChaser(const SatelliteWeak& chaser)
{
	if (mMethod)
	{
		mMethod->setChaser(chaser);
	}
}

void RendezvousControl::setTime(const Time& t)
{
	if (mMethod)
	{
		mMethod->setTime(t);
	}
}


SatelliteWeak RendezvousControl::getTarget() const
{
	if (mMethod)
	{
		return mMethod->getTarget();
	}

	return SatelliteShared(nullptr);
}

SatelliteWeak RendezvousControl::getChaser() const
{
	if (mMethod)
	{
		return mMethod->getChaser();
	}

	return SatelliteShared(nullptr);
}

Time RendezvousControl::getTime() const
{
	if (mMethod)
	{
		mMethod->getTime();
	}

	return Time();
}