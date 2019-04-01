#pragma once


#include <Model/Model.h>

#include <Shader/ShaderProgram.h>

#include <Objects/View.h>

#include <Graphics/GraphicsComponent.h>


class IRenderer
{
public:
	virtual void setRequiredStates() = 0;


    virtual void renderComponent(
		  const GraphicsComponentWeak& ptr
		, const View& view
	) = 0;


	virtual void renderModel(
		  const Model& model
		, const Mat4& data
	) = 0;


	virtual void renderNode(
		  const Model& model
		, UInt index
		, const Mat4& currentTransform
	) = 0;


	virtual void renderMesh(const Model& model, UInt index) = 0;


	virtual void restoreStates() = 0;
};

//aliases
using IRendererShared = std::shared_ptr<IRenderer>;
using IRendererWeak = std::weak_ptr<IRenderer>;