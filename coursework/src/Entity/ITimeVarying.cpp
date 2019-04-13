#include "ITimeVarying.h"

ITimeVarying::ITimeVarying(const Time& t)
	: mTime(t)
{}

const Time& ITimeVarying::time() const
{
	return mTime;
}