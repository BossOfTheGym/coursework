#pragma once


#include <Common.h>

#include "IObject.h"


struct NameComponent : public IComponent
{
public:
	NameComponent(IComponent* parent = nullptr);

	NameComponent(const NameComponent& comp) = default;

	NameComponent(NameComponent&& comp) = default;


	~NameComponent() = default;


	NameComponent& operator = (const NameComponent& comp) = default;

	NameComponent& operator = (NameComponent&& comp) = default;


public:
	virtual const Type& componentType() const override;


public:
	String mName;
};