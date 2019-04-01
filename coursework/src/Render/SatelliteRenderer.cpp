#include "SatelliteRenderer.h"

SatelliteRenderer::SatelliteRenderer(const ShaderProgramShared& shared)
	: mProgramShared(shared)
{
	if (mProgramShared)
	{
		setUniforms();
	}
}


void SatelliteRenderer::setRequiredStates()
{
	mProgramShared->use();	
	mProgramShared->setUniformVec3(uColor, Vec3(1.0f, 0.0f, 0.0f));
}


void SatelliteRenderer::renderComponent(const GraphicsComponentWeak& component, const View& view)
{
	//TODO : check if object has color attrib

	if (auto sharedGraphics = component.lock())
	{
		const auto& model = *(sharedGraphics->mModelPtr);
		const auto& data = sharedGraphics->mPhysicsPtr;

		if (auto sharedPhysics = sharedGraphics->mPhysicsPtr.lock())
		{
			mProgramShared->setUniformMat4(uView, view.view());
			mProgramShared->setUniformMat4(uProj, view.proj());

			renderModel(model, sharedPhysics->mMat);
		}
	}
}

void SatelliteRenderer::renderModel(const Model& model, const Mat4& transformation)
{
	renderNode(model, 0, transformation);
}

void SatelliteRenderer::renderNode(const Model& model, UInt index, const Mat4& currentTransform)
{
	const auto& node     = model.nodes()[index];

	const auto& children       = node.children();
	const auto& meshes         = node.meshes();
	const auto& transformations = model.transformations();

	auto nextTransform = currentTransform * transformations[index];

	for (UInt i = 0; i < children.size(); i++)
	{
		renderNode(model, children[i], nextTransform);
	}


	mProgramShared->setUniformMat4(uModel, nextTransform);
	for(UInt i = 0; i < meshes.size(); i++)
	{
		renderMesh(model, meshes[i]);
	}
}

void SatelliteRenderer::renderMesh(const Model& model, UInt index)
{
	const auto& mesh = model.meshes()[index];
	const auto& vab = mesh.vab();

	vab.bindArray();
	glDrawArrays(GL_TRIANGLES, 0, vab.elements());
}

void SatelliteRenderer::restoreStates()
{
	//restore, no need
}


void SatelliteRenderer::setUniforms()
{
	uModel = mProgramShared->getUniformLocation("model");
	uView  = mProgramShared->getUniformLocation("view");
	uProj  = mProgramShared->getUniformLocation("proj");
	uColor = mProgramShared->getUniformLocation("color");
 }