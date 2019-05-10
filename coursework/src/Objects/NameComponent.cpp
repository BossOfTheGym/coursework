#include "NameComponent.h"


NameComponent::NameComponent(IComponent* parent, const String& name)
	: IComponent(parent)
	, mName(name)
{}
