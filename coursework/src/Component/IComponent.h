#pragma once

#include <Common.h>

#include <Entity/ITimeVarying.h>


struct IComponent
{
public:
	using Type = String;


public:
	IComponent(IComponent* parent = nullptr);

	IComponent(const IComponent& comp) = default;

	IComponent(IComponent&& comp) = default;


	~IComponent() = default;


	IComponent& operator = (const IComponent& comp) = default;

	IComponent& operator = (IComponent&& comp) = default;


public:
	IComponent* owner();

	const IComponent* owner() const;


	virtual const Type& componentType() const = 0;


public:
	IComponent* mParent;
};


using IComponentShared = std::shared_ptr<IComponent>;
using IComponentWeak   = std::weak_ptr<IComponent>;