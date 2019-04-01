#pragma once


#include <Common.h>

#include "IRenderer.h"


class PlanetRenderer : public IRenderer
{
public:
	PlanetRenderer(const ShaderProgramShared& sharedProgram);

	PlanetRenderer(const PlanetRenderer& renderer) = default;

	PlanetRenderer(PlanetRenderer&& renderer) = default;


	~PlanetRenderer() = default;


	PlanetRenderer& operator = (const PlanetRenderer& renderer) = default;

	PlanetRenderer& operator = (PlanetRenderer&& renderer) = default;



	virtual void setRequiredStates() override;

	virtual void renderComponent(const GraphicsComponentWeak& component, const View& view) override;

	virtual void renderModel(const Model& model, const Mat4& data) override;

	virtual void renderNode(const Model& model, UInt index, const Mat4& currentTransform) override;

	virtual void renderMesh(const Model& model, UInt index) override;

	virtual void restoreStates() override;


private:
	void setUniforms();


private:
	ShaderProgramShared mProgramShared;

	//uniforms
	GLint uModel;
	GLint uView;
	GLint uProj;
};