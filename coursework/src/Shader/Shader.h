#pragma once

#include "../Common.h"


class Shader
{
private:
    static thread_local String INFO_LOG;


public:
    static const int ALL_SHADERS = 5;
    static const int BAD_SHADER  = 0;

    enum class Type : int
    {
          Vertex
        , TesselationControl
        , TesselationEvaluation
        , Geometry
        , Fragment
        , Invalid
    };


public:
    Shader();

    Shader(const Shader& shader) = delete;

    Shader(Shader&& shader);


    ~Shader();


    Shader& operator = (const Shader& shader) = delete;

    Shader& operator = (Shader&& shader);



    int loadFromLocation(Type type, const String& location);

    int loadFromStream(Type type, IStream& inputStream);

    int loadFromString(Type type, const String& source);


    void deleteShader();


    GLuint getId() const;

    Type getType() const;


    bool valid() const;

    bool compiled() const;


    const String& getInfoLog() const;

private:    
    GLuint m_id;
    Type m_type;


    void resetShader();
};

