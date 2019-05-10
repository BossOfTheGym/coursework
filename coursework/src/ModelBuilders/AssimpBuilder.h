#pragma once

#include <Model/Model.h>


class AssimpBuilder
{
public:
	using Importer = Assimp::Importer;

public:
	static const int DEFAULT_FLAGS = 
		  aiProcess_CalcTangentSpace 
		| aiProcess_Triangulate 
		| aiProcess_SortByPType 
		| aiProcess_OptimizeMeshes;


public:
	AssimpBuilder();

	AssimpBuilder(const AssimpBuilder& builder) = delete;

	AssimpBuilder(AssimpBuilder&& builder) = delete;


	~AssimpBuilder();


	AssimpBuilder& operator = (const AssimpBuilder& builder) = delete;

	AssimpBuilder& operator = (AssimpBuilder&& builder) = delete;



	int& flags();

	Importer& importer();

	Model&& imported();


	void readFile(const String& location);


private:
	Model mImported;
	Importer mImporter;
	int mImportFlags;
};