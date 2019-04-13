#include "IObject.h"

IObject::IObject(IComponent* parent, const Time& t)
	: IComponent(parent)
	, ITimeVarying(t)
{}