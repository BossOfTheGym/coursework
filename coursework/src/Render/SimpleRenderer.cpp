#include "SimpleRenderer.h"


SimpleRenderer::SimpleRenderer(const ShaderProgramShared& programShared)
	: mProgramShared(programShared)
	, mList()
{
	if (mProgramShared)
	{
		setUniforms();
	}
	mList.reserve(10);
}


void SimpleRenderer::setRequiredStates()
{
	mProgramShared->use();	
}

void SimpleRenderer::addToList(const IObjectWeak& obj)
{
	mList.push_back(obj);
}

void SimpleRenderer::render(const View& view)
{
	setRequiredStates();

	for (auto& obj : mList)
	{
		renderObject(obj, view);
	}

	restoreStates();
}

void SimpleRenderer::renderObject(const IObjectWeak& obj, const View& view)
{
	if (auto objPtr = obj.lock())
	{
		//here my logic crashes
	}
}

void SimpleRenderer::renderComponent(const GraphicsComponentWeak& component, const View& view)
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

void SimpleRenderer::renderModel(const Model& model, const Mat4& transformation)
{
	renderNode(model, 0, transformation);
}

void SimpleRenderer::renderNode(const Model& model, UInt index, const Mat4& currentTransform)
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

void SimpleRenderer::renderMesh(const Model& model, UInt index)
{
	const auto& mesh = model.meshes()[index];
	const auto& vab = mesh.vab();

	vab.bind();
	glDrawArrays(GL_TRIANGLES, 0, vab.primitivesCount());
}

void SimpleRenderer::restoreStates()
{
	//restore, no need
}


void SimpleRenderer::setUniforms()
{
	uModel = mProgramShared->getUniformLocation("model");
	uView  = mProgramShared->getUniformLocation("view");
	uProj  = mProgramShared->getUniformLocation("proj");
}