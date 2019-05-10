#pragma once

#include <Common.h>

#include <Model/Model.h>

#include <Component/IComponent.h>
#include <Physics/PhysicsComponent.h>



struct GraphicsComponent : public IComponent
{
public:
	static const size_t DEFAULT_DATA_SIZE = 10;


public:
	GraphicsComponent(
		IComponent* parent = nullptr
		, const ModelShared& modelPtr = ModelShared(nullptr)
		, const PhysicsComponentWeak& physicsComp = PhysicsComponentShared(nullptr)
	);

	GraphicsComponent(const GraphicsComponent& comp) = default;
	GraphicsComponent(GraphicsComponent&& comp)      = default;

	virtual ~GraphicsComponent() = default;

	GraphicsComponent& operator = (const GraphicsComponent& comp) = default;
	GraphicsComponent& operator = (GraphicsComponent&& comp) = default;


public:
	ModelShared mModelPtr;
	PhysicsComponentWeak mPhysicsPtr;
};


//aliases
using GraphicsComponentShared = std::shared_ptr<GraphicsComponent>;
using GraphicsComponentWeak = std::weak_ptr<GraphicsComponent>;