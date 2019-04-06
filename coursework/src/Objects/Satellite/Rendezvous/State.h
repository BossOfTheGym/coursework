#pragma once


#include <Common.h>



struct State
{

};


//aliases
using StateShared = std::shared_ptr<State>;
using StateWeak   = std::weak_ptr<State>;
