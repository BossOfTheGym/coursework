#include "AssimpModel.h"


//constructors & destructor
AssimpMaterial::AssimpMaterial()
	: mNumDiffuse(0)
	, mDiffuse(nullptr)
	, mDiffusePtrs(nullptr)
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
		std::swap(mDiffusePtrs, material.mDiffusePtrs);
	}

	return *this;
}


//IObject
const String& AssimpMaterial::toString() const
{
	return mName;
}


//IMaterial
const UInt& AssimpMaterial::numDiffuse() const
{
	return mNumDiffuse;
}

const Texture2D** AssimpMaterial::diffuse() const
{
	return (const Texture2D**)mDiffusePtrs.get();
}


//load
void AssimpMaterial::loadMaterial(const aiMaterial* material)
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


	mDiffusePtrs.reset(mNumDiffuse ? new Texture2D*[mNumDiffuse] : nullptr);
	for (UInt i = 0; i < mNumDiffuse; i++)
	{
		mDiffusePtrs[i] = &mDiffuse[i];
	}
}