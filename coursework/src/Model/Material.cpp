#include "Model.h"


//constructors & destructor
Material::Material()	
	: mDiffuse()
{}

Material::Material(Textures&& diffuse)
	: mDiffuse(std::move(diffuse))
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



//Material
const Material::Textures& Material::diffuse() const
{
	return mDiffuse;
}
