#include "Time.h"

Time::Time(uint64_t timeInt, float timeFloat)
	: mTimeInt(timeInt)
	, mTimeFloat(timeFloat)
{}

uint64_t Time::asU64() const
{
	return mTimeInt;
}

float Time::asFloat() const
{
	return mTimeFloat;
}

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


bool Time::operator == (const Time& t) const
{
	return mTimeInt == t.mTimeInt;
}

bool Time::operator <= (const Time& t) const
{
	return mTimeInt <= t.mTimeInt;
}

bool Time::operator >= (const Time& t) const
{
	return mTimeInt >= t.mTimeInt;
}

bool Time::operator < (const Time& t) const
{
	return mTimeInt < t.mTimeInt;
}

bool Time::operator > (const Time& t) const
{
	return mTimeInt > t.mTimeInt;
}
