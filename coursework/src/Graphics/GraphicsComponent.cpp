#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent(
	  IComponent* parent
	, const ModelShared& modelPtr
	, const PhysicsComponentWeak& physicsComp
)
	: IComponent(parent)
	, mModelPtr(modelPtr)
	, mPhysicsPtr(physicsComp)
{
}


const IComponent::Type& GraphicsComponent::componentType() const
{
	static const IComponent::Type type = "Graphics";

	return type;
}
