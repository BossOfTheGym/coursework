#include "NameComponent.h"


NameComponent::NameComponent(IComponent* parent)
	: IComponent(parent)
	, mName("")
{}

const IComponent::Type& NameComponent::componentType() const
{
	static const Type type = "Name";

	return type;
}