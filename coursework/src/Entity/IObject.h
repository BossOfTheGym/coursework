#pragma once

#include <Common.h>


class IObject
{
public:
    virtual const String& toString() const = 0;
};