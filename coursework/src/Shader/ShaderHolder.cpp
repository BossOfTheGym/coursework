#include "ShaderHolder.h"


//statics
ShaderHolder::Pointer ShaderHolder::instance;


ShaderHolder& ShaderHolder::getInstance()
{
    if (!instance)
    {
        instance = Pointer(new ShaderHolder);
    }

    return *instance;
}

void ShaderHolder::freeInstance()
{
    delete instance;
}


//constructor
ShaderHolder::ShaderHolder() : m_shaders()
{}


//load
void ShaderHolder::loadShader(ShaderType::Type type, const String& location)
{
    m_shaders[location] = std::move(Shader(type, location));
}

void ShaderHolder::deleteShader(const String& location)
{
    m_shaders.erase(location);
}
