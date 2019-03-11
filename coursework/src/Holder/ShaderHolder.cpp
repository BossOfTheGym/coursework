#include "ShaderHolder.h"


//statics
ShaderHolder::HolderPointer ShaderHolder::instance;


ShaderHolder& ShaderHolder::getInstance()
{
    if (!instance)
    {
        instance = HolderPointer(new ShaderHolder);
    }

    return *instance;
}

void ShaderHolder::freeInstance()
{
    instance.reset(nullptr);
}


//constructor
ShaderHolder::ShaderHolder() : m_shaders()
{}


//load
void ShaderHolder::loadShader(Shader::Type type, const String& location)
{
    m_shaders[location] = ShaderPointer(new Shader(type, location));
}

void ShaderHolder::deleteShader(const String& location)
{
    m_shaders.erase(location);
}


//operators
ShaderHolder::ShaderPointer ShaderHolder::operator[](const String& location)
{
    return m_shaders[location];
}
