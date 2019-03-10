#pragma once


#include <map>
#include <memory>

#include "../Common.h"

#include "Shader.h"


class ShaderHolder
{
public:
    using Key   = String;
    using Value = Shader;

    using Pointer = std::unique_ptr<ShaderHolder>;


public:
    static ShaderHolder& getInstance();

    static void freeInstance();


public:
    Shader& operator [] (const String& location);

    const Shader operator [] (const String& location) const;


    void loadShader(ShaderType::Type type, const String& location);

    void deleteShader(const String& location);


private:
    static Pointer instance;


private:
    ShaderHolder();

    ShaderHolder(const ShaderHolder& holder) = delete;

    ShaderHolder(ShaderHolder&& holder) = delete;


    ShaderHolder& operator = (const ShaderHolder& holder) = delete;

    ShaderHolder&& operator = (ShaderHolder&& holder) = delete;


private:
    std::map<Key, Value> m_shaders;
};