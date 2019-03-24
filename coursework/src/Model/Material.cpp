#include "Model.h"


//statics
Material Material::buildFromAssimp(const aiMaterial* material, const String& name)
{
	Material result;

	result.mDiffuse.resize(material->GetTextureCount(aiTextureType_DIFFUSE));
	
	//diffuse
	aiString path;
	for (UInt i = 0; i < result.mDiffuse.size(); i++)
	{
		if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS)
		{
			result.mDiffuse[i] = Texture2D(path.C_Str());
		}
	}

	return result;
}

Material Material::buildFromData()
{
	return Material();
}


//constructors & destructor
Material::Material()	
	: mDiffuse()
	, mName("")
{}

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
		std::swap(mDiffuse, material.mDiffuse);
	}

	return *this;
}


//IObject
const String& Material::toString() const
{
	return mName;
}


//Material
const Material::Textures& Material::diffuse() const
{
	return mDiffuse;
}
