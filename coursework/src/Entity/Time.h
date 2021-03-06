#pragma once

#include <Common.h>


class Time
{
public:
	Time(uint64_t timeInt = 0, double timeFloat = 0.0f);

	Time(const Time& time) = default;
	Time(Time&& time)      = default;

	~Time() = default;

	Time& operator = (const Time& time) = default;
	Time& operator = (Time&& time)      = default;


public:
	uint64_t asU64() const;

	double asFloat() const;


public:
	Time operator - (const Time& t) const;
	Time operator + (const Time& t) const;
	
	Time operator * (uint64_t factor) const;

	Time& operator -= (const Time& t);
	Time& operator += (const Time& t);

	bool operator == (const Time& t) const;
	bool operator <= (const Time& t) const;
	bool operator >= (const Time& t) const;
	bool operator < (const Time& t) const;
	bool operator > (const Time& t) const;

private:
	uint64_t mTimeInt;
	double    mTimeFloat;
};