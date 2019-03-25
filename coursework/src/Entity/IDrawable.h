#pragma once


#include <Model/Model.h>


class IDrawable
{
public:
	virtual const Model& model() const = 0;
};