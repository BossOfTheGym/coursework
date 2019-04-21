#include "PlanetRenderer.h"


PlanetRenderer::PlanetRenderer(const ShaderProgramShared& sharedProgram)
	: mProgramShared(sharedProgram)
	, mList()
{
	if (sharedProgram)
	{
		setUniforms();
	}
	mList.reserve(10);
}


void PlanetRenderer::setRequiredStates()
{
	//set required states
	mProgramShared->use();
}

void PlanetRenderer::addToList(const IObjectWeak& obj)
{
	mList.push_back(obj);
}

void PlanetRenderer::render(const View& view)
{
	setRequiredStates();

	for (auto& obj : mList)
	{
		renderObject(obj, view);
	}

	restoreStates();
}

void PlanetRenderer::renderObject(const IObjectWeak& obj, const View& view)
{
	if (auto objPtr = std::dynamic_pointer_cast<Planet>(obj.lock()))
	{
		renderComponent(objPtr->mGraphics, view);
	}
}

void PlanetRenderer::renderComponent(const GraphicsComponentWeak& component, const View& view)
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

void PlanetRenderer::renderModel(const Model& model, const Mat4& transform)
{
	renderNode(model, 0, transform);
}

void PlanetRenderer::renderNode(const Model& model, UInt index, const Mat4& currentTransform)
{
	const auto& node     = model.nodes()[index];

	const auto& children        = node.children();
	const auto& meshes          = node.meshes();
	const auto& transformations = model.transformations();

	auto nextTransform = currentTransform * transformations[index];

	for (UInt i = 0; i < children.size(); i++)
	{
		renderNode(model, children[i], nextTransform);
	}


	mProgramShared->setUniformMat4(uModel, nextTransform);
	mProgramShared->setUniformMat4(uInvTrans, glm::transpose(glm::inverse(nextTransform)));
	for(UInt i = 0; i < meshes.size(); i++)
	{
		renderMesh(model, meshes[i]);
	}
}

void PlanetRenderer::renderMesh(const Model& model, UInt index)
{
	const auto& mesh = model.meshes()[index];
	const auto& vab = mesh.vab();
	const auto& i = mesh.material();

	const auto& diffuse = (model.materials()[i]).diffuse()[0];


	//apply material
	Texture2D::active(GL_TEXTURE0);
	diffuse.bind();

	vab.bindArray();
	glDrawArrays(GL_PATCHES, 0, vab.elements());
}

void PlanetRenderer::restoreStates()
{
	//restore states that were changed
}


void PlanetRenderer::setUniforms()
{
	uModel = mProgramShared->getUniformLocation("model");
	uView  = mProgramShared->getUniformLocation("view");
	uProj  = mProgramShared->getUniformLocation("proj");
	uInvTrans = mProgramShared->getUniformLocation("invTrans");
}