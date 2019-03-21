#include "ShaderProgram.h"


//statics
thread_local String ShaderProgram::INFO_LOG;


//constructors & destructor
ShaderProgram::ShaderProgram()
	: mId(EMPTY)
	, mName("")
{}

ShaderProgram::ShaderProgram(const String& name) 
    : mId(glCreateProgram())
    , mName(name)
{}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
{
	*this = std::move(shaderProgram);
}


ShaderProgram::~ShaderProgram()
{
    deleteProgram();
}


//operators
ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram)
{
	if (this != &shaderProgram)
	{
		std::swap(mId, shaderProgram.mId);
	}

    return *this;
}


//core functions
void ShaderProgram::attachShader(const Shader& shader)
{
	std::cout << "Attached: " << shader.getId() << std::endl;
    glAttachShader(mId, shader.getId());
}

void ShaderProgram::detachShader(const Shader& shader)
{
    glDetachShader(mId, shader.getId());
}


void ShaderProgram::link()
{
    glLinkProgram(mId);
}

void ShaderProgram::use()
{
    glUseProgram(mId);
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
    return glGetUniformLocation(mId, name.c_str());
}

GLint ShaderProgram::getAttributeLocation(const String& name)
{
    return glGetAttribLocation(mId, name.c_str());
}


//delete & reset
void ShaderProgram::deleteProgram()
{
    glDeleteProgram(mId);

    resetProgram();
}

void ShaderProgram::resetProgram()
{
    mId = EMPTY;
    mName = "";
}



//fake virtual
const String& ShaderProgram::toString() const
{
    return mName;
}

//checks
bool ShaderProgram::valid()
{
    return mId == EMPTY;
}

bool ShaderProgram::linked()
{
    GLint result;

    glGetProgramiv(mId, GL_LINK_STATUS, &result);

    return result == GL_TRUE;
}


//get
GLuint ShaderProgram::getId() const
{
    return mId;
}


//info log
const String& ShaderProgram::getInfoLog()
{
    String& infoLog = ShaderProgram::INFO_LOG;

    GLint length;
    GLint returned;

    glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &length);
    infoLog.resize(length);

    glGetProgramInfoLog(mId, length, &returned, infoLog.data());
    infoLog.resize(returned);

    return infoLog;
}