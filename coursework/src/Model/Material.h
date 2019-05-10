#pragma once


#include <assimp/scene.h>

#include <Common.h>

#include <glWrap/Texture/Texture2D.h>


class Material
{
public:
	using Textures = std::vector<Texture2D>;


public:
	Material();

	Material(Textures&& diffuse);

	Material(const Material& material) = delete;

	Material(Material&& material);


	~Material();


	Material& operator = (const Material& material) = delete;

	Material& operator = (Material&& material);


	//Material
	const Textures& diffuse() const;


private:
	Textures mDiffuse;
};