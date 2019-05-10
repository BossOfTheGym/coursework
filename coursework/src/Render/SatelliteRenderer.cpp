#include "SatelliteRenderer.h"

SatelliteRenderer::SatelliteRenderer(const ShaderProgramShared& shared)
	: mProgramShared(shared)
	, mList()
{
	if (mProgramShared)
	{
		setUniforms();
	}
	mList.reserve(10);
}


void SatelliteRenderer::setRequiredStates()
{
	mProgramShared->use();	
}

void SatelliteRenderer::addToList(const IObjectWeak& obj)
{
	mList.push_back(obj);
}

void SatelliteRenderer::render(const View& view)
{
	setRequiredStates();

	for (auto& obj : mList)
	{
		renderObject(obj, view);
	}

	restoreStates();
}

void SatelliteRenderer::renderObject(const IObjectWeak& obj, const View& view)
{
	if(auto objPtr = std::dynamic_pointer_cast<Satellite>(obj.lock()))
	{
		mProgramShared->setUniformVec3(uColor, objPtr->mSatellite->mColor);

		renderComponent(objPtr->mGraphics, view);
	}
}

void SatelliteRenderer::renderComponent(const GraphicsComponentWeak& component, const View& view)
{
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

	vab.bind();
	glDrawArrays(GL_TRIANGLES, 0, vab.primitivesCount());
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