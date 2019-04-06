#pragma once


#include <Common.h>

#include <Entity/ITimeVarying.h>

#include <Component/IComponent.h>


struct IObject : public IComponent, public ITimeVarying
{
};

using IObjectShared = std::shared_ptr<IObject>;
using IObjectWeak = std::weak_ptr<IObject>;