#pragma once

#include "Shader.h"


class ShaderProgram
{
private:
    static thread_local String INFO_LOG;


public:
    static const GLuint EMPTY = 0;


public:
    ShaderProgram(const String& name = "");

    ShaderProgram(const ShaderProgram& shaderProgram) = delete;

    ShaderProgram(ShaderProgram&& shaderProgram);


    ~ShaderProgram();


    ShaderProgram& operator = (const ShaderProgram& shaderProgram) = delete;

    ShaderProgram& operator = (ShaderProgram&& shaderProgram);



    void attachShader(const Shader& shader);

    void detachShader(const Shader& shader);


    void link();

    void use();



    void setUniformMat4(GLint location, const Mat4& mat);

    void setUniformMat3(GLint location, const Mat3& mat);


    void setUniformVec4(GLint location, const Vec4& vec);

    void setUniformVec3(GLint location, const Vec3& vec);


    void setUniform1f(GLint location, float value);


    GLint getUniformLocation(const String& name);

    GLint getAttributeLocation(const String& name);



    void deleteProgram();


    bool valid();

    bool linked();



    GLuint getId() const;
    
    const String& getInfoLog();


    virtual const String& toString() const;

private:
    void resetProgram();


private:
    GLuint mId;

    String mName;
};