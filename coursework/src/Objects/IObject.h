#pragma once


#include <Common.h>

#include <Entity/ITimeVarying.h>

#include <Component/IComponent.h>


struct IObject : public IComponent, public ITimeVarying
{
	IObject(IComponent* parent = nullptr, const Time& t = Time());

	IObject(const IObject& obj) = default;

	IObject(IObject&& obj) = default;

	
	virtual ~IObject() = default;


	IObject& operator = (const IObject& obj) = default;

	IObject& operator = (IObject&& obj) = default;
};

using IObjectShared = std::shared_ptr<IObject>;
using IObjectWeak = std::weak_ptr<IObject>;