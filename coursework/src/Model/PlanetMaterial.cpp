//#include "PlanetModel.h"
//
//
////statics
//const UInt PlanetMaterial::NUM_DIFFUSE = 1;
//
//
////constructor & destructor
//PlanetMaterial::PlanetMaterial()
//	: mName("")
//	, mDiffuse()
//	, mDiffusePtr(&mDiffuse)
//{}
//
//PlanetMaterial::PlanetMaterial(
//	  const String& diffuseLocation
//	, const String& name
//)
//	: mName(name)
//	, mDiffuse(diffuseLocation)
//	, mDiffusePtr(&mDiffuse)
//{
//	mDiffuse.bind();
//	mDiffuse.texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP);
//	mDiffuse.texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP);
//	mDiffuse.texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	mDiffuse.texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}
//
//PlanetMaterial::PlanetMaterial(PlanetMaterial&& material) : PlanetMaterial()
//{
//	*this = std::move(material);
//}
//
//
//PlanetMaterial::~PlanetMaterial()
//{}
//
//
////operators
//PlanetMaterial& PlanetMaterial::operator = (PlanetMaterial&& material)
//{
//	if (this != &material)
//	{
//		std::swap(mName, material.mName);
//		std::swap(mDiffuse, material.mDiffuse);
//	}
//
//	return *this;
//}
//
//
////IObject
//const String& PlanetMaterial::toString() const
//{
//	return mName;
//}
//
//
////IMaterial
//const UInt& PlanetMaterial::numDiffuse() const
//{
//	return NUM_DIFFUSE;
//}
//
//const Texture2D** PlanetMaterial::diffuse() const
//{
//	return (const Texture2D**)&mDiffusePtr;
//}