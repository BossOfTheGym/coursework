#pragma once

#include "Time.h"

class ITimeVarying
{
public:
	ITimeVarying(const Time& t = Time());

	ITimeVarying(const ITimeVarying& itv) = default;
	ITimeVarying(ITimeVarying&& itv)      = default;

	virtual ~ITimeVarying() = default;

	ITimeVarying& operator = (const ITimeVarying& itv) = default;
	ITimeVarying& operator = (ITimeVarying&& itv)      = default;


public:
    virtual void update(const Time& t) = 0;


public:
	const Time& time() const;


protected:
	Time mTime;
};