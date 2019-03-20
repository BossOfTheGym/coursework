#include "Model.h"


//constructors & destructor
Material::Material()
	: mNumDiffuse()
	, mDiffuse()
	, mName("")
{}

Material::Material(const aiMaterial* material, const String& name) : Material()
{
	if (material)
	{
		loadMaterial(material);
	}
}

Material::Material(Material&& material)
{
	*this = std::move(material);
}


Material::~Material()
{}


//operators
Material& Material::operator = (Material&& material)
{
	if (this != &material)
	{
		std::swap(mNumDiffuse, material.mNumDiffuse);
		std::swap(mDiffuse, material.mDiffuse);
	}

	return *this;
}


//IObjectBase
const String& Material::toString() const
{
	return mName;
}


//get & set
const UInt& Material::numDiffuse(const String& path) const
{
	return mNumDiffuse;
}

const Material::Textures& Material::diffuse()
{
	return mDiffuse;
}


//private
void Material::loadMaterial(const aiMaterial* material)
{
	mNumDiffuse = material->GetTextureCount(aiTextureType_DIFFUSE);
	mDiffuse.reset(mNumDiffuse ? new Texture2D[mNumDiffuse] : nullptr);

	aiString path;
	for (UInt i = 0; i < mNumDiffuse; i++)
	{
		if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS)
		{
			mDiffuse[i] = Texture2D(path.C_Str());
		}
	}
}