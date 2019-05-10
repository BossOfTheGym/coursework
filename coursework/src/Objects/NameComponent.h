#pragma once


#include <Common.h>

#include "IObject.h"


struct NameComponent : public IComponent
{
public:
	NameComponent(
		IComponent* parent = nullptr
		, const String& name = ""
	);

	NameComponent(const NameComponent& comp) = default;
	NameComponent(NameComponent&& comp)      = default;

	virtual ~NameComponent() = default;

	NameComponent& operator = (const NameComponent& comp) = default;
	NameComponent& operator = (NameComponent&& comp)      = default;


public:
	String mName;
};


//alias
using NameComponentShared = std::shared_ptr<NameComponent>;
using NameComponentWeak   = std::weak_ptr<NameComponent>;