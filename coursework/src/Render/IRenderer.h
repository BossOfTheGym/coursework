#pragma once


#include <Model/Model.h>

#include <glWrap/Shader/ShaderProgram.h>

#include <Objects/View.h>
#include <Objects/IObject.h>

#include <Graphics/GraphicsComponent.h>


class IRenderer
{
public:
	virtual void setRequiredStates() = 0;

	//add object to render list
	virtual void addToList(const IObjectWeak& obj) = 0;

	//render all objects
	virtual void render(const View& view) = 0;


	virtual void renderObject(
		const IObjectWeak& obj
		, const View& view
	) = 0;


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