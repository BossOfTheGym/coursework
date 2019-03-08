#pragma once

#include "../Common.h"


class Shader
{
private:
    static thread_local String INFO_LOG;


public:
    static const GLuint EMPTY  = 0;


public:
    enum Type : GLenum
    {
          Vertex         = GL_VERTEX_SHADER
        , TessControl    = GL_TESS_CONTROL_SHADER
        , TessEvaluation = GL_TESS_EVALUATION_SHADER
        , Geometry       = GL_GEOMETRY_SHADER
        , Fragment       = GL_FRAGMENT_SHADER
        , Invalid = 0
    };


public:
    Shader(Type type = Invalid, const String& location = "");

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
    void resetShader();


private:    
    GLuint m_id;
    Type m_type;
};

