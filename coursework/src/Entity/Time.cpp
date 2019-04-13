#include "Time.h"

Time::Time(uint64_t timeInt, float timeFloat)
	: mTimeInt(timeInt)
	, mTimeFloat(timeFloat)
{}


Time Time::operator - (const Time& t) const
{
	return Time(mTimeInt - t.mTimeInt, mTimeFloat - t.mTimeFloat);
}

Time Time::operator + (const Time& t) const
{
	return Time(mTimeInt + t.mTimeInt, mTimeFloat + t.mTimeFloat);
}

Time Time::operator * (uint64_t factor) const
{
	return Time(mTimeInt * factor, mTimeFloat * factor);
}


Time& Time::operator -= (const Time& t)
{
	mTimeFloat -= t.mTimeFloat;
	mTimeInt   -= t.mTimeInt;

	return *this;
}

Time& Time::operator += (const Time& t)
{
	mTimeFloat += t.mTimeFloat;
	mTimeInt   += t.mTimeInt;

	return *this;
}