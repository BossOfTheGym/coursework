#pragma once

#include <Common.h>


class INameable
{
public:
    virtual const String& toString() const = 0;
};