#include "IComponent.h"


IComponent::IComponent(IComponent* parent)
	: mParent(parent)
{}


IComponent* IComponent::owner()
{
	return mParent;
}

const IComponent* IComponent::owner() const
{
	return mParent;
}