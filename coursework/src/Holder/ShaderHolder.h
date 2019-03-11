#pragma once


#include <map>
#include <memory>

#include "../Common.h"

#include "../Shader/Shader.h"


class ShaderHolder
{
public:
    using Key   = String;
    using Value = Shader;

    using ShaderPointer = std::shared_ptr<Value>;
    using HolderPointer = std::unique_ptr<ShaderHolder>;


public:
    static ShaderHolder& getInstance();

    static void freeInstance();


public:
    ShaderPointer operator [] (const String& location);


    void loadShader(Shader::Type type, const String& location);

    void deleteShader(const String& location);


private:
    static HolderPointer instance;


private:
    ShaderHolder();

    ShaderHolder(const ShaderHolder& holder) = delete;

    ShaderHolder(ShaderHolder&& holder) = delete;


    ShaderHolder& operator = (const ShaderHolder& holder) = delete;

    ShaderHolder&& operator = (ShaderHolder&& holder) = delete;


private:
    std::map<Key, ShaderPointer> m_shaders;
};