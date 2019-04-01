#pragma once


#include <Common.h>

#include "IRenderer.h"


class SatelliteRenderer : public IRenderer
{
public:
	SatelliteRenderer(const ShaderProgramShared& programShared);

	SatelliteRenderer(const SatelliteRenderer& renderer) = default;

	SatelliteRenderer(SatelliteRenderer&& rendrer) = default;


	~SatelliteRenderer() = default;


	SatelliteRenderer& operator = (const SatelliteRenderer& renderer) = default;

	SatelliteRenderer& operator = (SatelliteRenderer&& rendrer) = default;



	virtual void setRequiredStates() override;

	virtual void renderComponent(const GraphicsComponentWeak& component, const View& view) override;

	virtual void renderModel(const Model& model, const Mat4& transformation) override;

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
	GLint uColor;
};
