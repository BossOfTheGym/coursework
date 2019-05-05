#include "Method.h"


Method::Method(const SatelliteWeak& target, const SatelliteWeak& chaser)
	: mTarget(target)
	, mChaser(chaser)
{}

void Method::start()
{}

void Method::stop()
{}

void Method::update(const Time& t, const Time& dt)
{}

bool Method::finished() const
{
	return true;
}


void Method::setTarget(const SatelliteWeak& target)
{
	mTarget = target;
}

void Method::setChaser(const SatelliteWeak& chaser)
{
	mChaser = chaser;
}

void Method::setTime(const Time& t)
{
	mTime = t;
}


SatelliteWeak Method::getTarget() const
{
	return mTarget;
}

SatelliteWeak Method::getChaser() const
{
	return mChaser;
}

Time Method::getTime() const
{
	return mTime;
}
