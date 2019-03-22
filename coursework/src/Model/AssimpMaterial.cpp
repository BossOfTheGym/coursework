#include "AssimpModel.h"


//constructors & destructor
AssimpMaterial::AssimpMaterial()
	: mNumDiffuse()
	, mDiffuse()
	, mName("")
{}

AssimpMaterial::AssimpMaterial(const aiMaterial* material, const String& name) : AssimpMaterial()
{
	if (material)
	{
		loadMaterial(material);
	}
}

AssimpMaterial::AssimpMaterial(AssimpMaterial&& material)
{
	*this = std::move(material);
}


AssimpMaterial::~AssimpMaterial()
{}


//operators
AssimpMaterial& AssimpMaterial::operator = (AssimpMaterial&& material)
{
	if (this != &material)
	{
		std::swap(mNumDiffuse, material.mNumDiffuse);
		std::swap(mDiffuse, material.mDiffuse);
	}

	return *this;
}


//IObjectBase
const String& AssimpMaterial::toString() const
{
	return mName;
}


//get & set
const UInt& AssimpMaterial::numDiffuse() const
{
	return mNumDiffuse;
}

const Texture2D* AssimpMaterial::diffuse() const
{
	return mDiffuse.get();
}


//private
void AssimpMaterial::loadMaterial(const aiMaterial* material)
{
	mNumDiffuse = material->GetTextureCount(aiTextureType_DIFFUSE);
	mDiffuse.reset(mNumDiffuse ? new Texture2D[mNumDiffuse] : nullptr);

	aiString path;
	for (UInt i = 0; i < mNumDiffuse; i++)
	{
		if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS)
		{
			std::cerr << path.C_Str() << std::endl;
			mDiffuse[i] = Texture2D(path.C_Str());
		}
	}
}