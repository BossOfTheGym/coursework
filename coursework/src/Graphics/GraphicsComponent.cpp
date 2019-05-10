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
