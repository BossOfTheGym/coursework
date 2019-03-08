#include "ShaderProgram.h"


//statics
thread_local String ShaderProgram::INFO_LOG;


//constructors & destructor
ShaderProgram::ShaderProgram() : m_id(glCreateProgram())
{}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
{
    m_id = shaderProgram.m_id;

    shaderProgram.resetProgram();
}


ShaderProgram::~ShaderProgram()
{
    deleteProgram();
}


//operators
ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram)
{
    deleteProgram();

    m_id = shaderProgram.m_id;

    shaderProgram.resetProgram();

    return *this;
}


//core functions
void ShaderProgram::attachShader(const Shader& shader)
{
    glAttachShader(m_id, shader.getId());
}

void ShaderProgram::detachShader(const Shader& shader)
{
    glDetachShader(m_id, shader.getId());
}


void ShaderProgram::link()
{
    glLinkProgram(m_id);
}

void ShaderProgram::use()
{
    glUseProgram(m_id);
}


//uniforms
void ShaderProgram::setUniformMat4(GLint location, const Mat4& mat)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformMat3(GLint location, const Mat3& mat)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}


void ShaderProgram::setUniformVec4(GLint location, const Vec4& vec)
{
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniformVec3(GLint location, const Vec3& vec)
{
    glUniform3fv(location, 1, glm::value_ptr(vec));
}


void ShaderProgram::setUniform1f(GLint location, float value)
{
    glUniform1f(location, value);
}


//locations
GLint ShaderProgram::getUniformLocation(const String& name)
{
    return glGetUniformLocation(m_id, name.c_str());
}

GLint ShaderProgram::getAttributeLocation(const String& name)
{
    return glGetAttribLocation(m_id, name.c_str());
}


//delete & reset
void ShaderProgram::deleteProgram()
{
    glDeleteProgram(m_id);

    resetProgram();
}

void ShaderProgram::resetProgram()
{
    m_id = EMPTY;
}


//checks
bool ShaderProgram::valid()
{
    return m_id == EMPTY;
}

bool ShaderProgram::linked()
{
    GLint result;

    glGetProgramiv(m_id, GL_LINK_STATUS, &result);

    return result == GL_TRUE;
}


//get
GLuint ShaderProgram::getId() const
{
    return m_id;
}


//info log
const String& ShaderProgram::getInfoLog()
{
    String& infoLog = ShaderProgram::INFO_LOG;

    GLint length;
    GLint returned;

    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
    infoLog.resize(length);

    glGetProgramInfoLog(m_id, length, &returned, infoLog.data());
    infoLog.resize(returned);

    return infoLog;
}