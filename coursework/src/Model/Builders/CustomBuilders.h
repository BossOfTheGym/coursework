#pragma once

#include <Model/Model.h>


class PlanetBuilder
{
public:
	PlanetBuilder();

	PlanetBuilder(const PlanetBuilder& builder) = delete;

	PlanetBuilder(PlanetBuilder&& builder) = delete;


	~PlanetBuilder();


	PlanetBuilder& operator = (const PlanetBuilder& builder) = delete;

	PlanetBuilder&& operator = (PlanetBuilder&& builder) = delete;


	void build(
		UInt triangleSplit
		, const String& diffuse
		, const String& name
	);

	Model&& model();

private:
	Model mBuilt;
};


class BoxBuilder
{
public:
	BoxBuilder();

	BoxBuilder(const BoxBuilder& builder) = delete;

	BoxBuilder(BoxBuilder&& builder) = delete;


	~BoxBuilder();


	BoxBuilder& operator = (const BoxBuilder& builder) = delete;

	BoxBuilder& operator = (BoxBuilder&& builder) = delete;



	void build(const String& name);

	Model&& model();


private:
	Model mBuilt;
};