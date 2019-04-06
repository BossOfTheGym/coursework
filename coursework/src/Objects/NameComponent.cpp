#include "NameComponent.h"


NameComponent::NameComponent(IComponent* parent, const String& name)
	: IComponent(parent)
	, mName(name)
{}

const IComponent::Type& NameComponent::componentType() const
{
	static const Type type = "Name";

	return type;
}