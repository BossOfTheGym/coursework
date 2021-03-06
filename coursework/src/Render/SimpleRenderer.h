#pragma once

\
#include <Common.h>

#include "IRenderer.h"


class SimpleRenderer : public IRenderer
{
public:
	SimpleRenderer(const ShaderProgramShared& programShared);

	SimpleRenderer(const SimpleRenderer& renderer) = default;

	SimpleRenderer(SimpleRenderer&& rendrer) = default;


	~SimpleRenderer() = default;


	SimpleRenderer& operator = (const SimpleRenderer& renderer) = default;

	SimpleRenderer& operator = (SimpleRenderer&& rendrer) = default;



	virtual void setRequiredStates() override;

	virtual void addToList(const IObjectWeak& obj) override;

	virtual void render(const View& view) override;

	virtual void renderObject(const IObjectWeak& obj, const View& view) override;

	virtual void renderComponent(const GraphicsComponentWeak& component, const View& view) override;

	virtual void renderModel(const Model& model, const Mat4& transformation) override;

	virtual void renderNode(const Model& model, UInt index, const Mat4& currentTransform) override;

	virtual void renderMesh(const Model& model, UInt index) override;

	virtual void restoreStates() override;


private:
	void setUniforms();


private:
	std::vector<IObjectWeak> mList;

	ShaderProgramShared mProgramShared;

	//uniforms
	GLint uModel;
	GLint uView;
	GLint uProj;
};
	