#include "Model.h"


//constructors & destructor
Material::Material()	
	: mDiffuse()
	, mName("")
{}

Material::Material(Textures&& diffuse, String&& name)
	: mDiffuse(std::move(diffuse))
	, mName(std::move(name))
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


//INameable
const String& Material::toString() const
{
	return mName;
}


//Material
const Material::Textures& Material::diffuse() const
{
	return mDiffuse;
}
