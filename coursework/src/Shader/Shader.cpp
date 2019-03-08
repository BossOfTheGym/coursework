

#include "Shader.h"


//statics
thread_local String Shader::INFO_LOG;


//constructors
Shader::Shader() : m_id(EMPTY), m_type(Type::Invalid)
{}

Shader::Shader(Shader&& shader)
{
    m_id   = shader.m_id;
    m_type = shader.m_type;

    shader.resetShader();
}


//destructor
Shader::~Shader()
{
    deleteShader();
}


//operators
Shader& Shader::operator = (Shader&& shader)
{
    deleteShader();

    m_id   = shader.m_id;
    m_type = shader.m_type;

    shader.resetShader();

    return *this;
}


//load functions
int Shader::loadFromLocation(Type type, const String& location)
{
    IFStream file(location);

    if (!file.is_open())
    {
        return false;
    }

    return loadFromStream(type, file);
}

int Shader::loadFromStream(Type type, IStream& inputStream)
{
    String source;
    String line;

    while (inputStream.good())
    {
        std::getline(inputStream, line);

        source += line;
        source += '\n';
    }

    if (!inputStream.eof())
    {
        return false;
    }

    return loadFromString(type, source);
}

int Shader::loadFromString(Type type, const String& source)
{
    if (type == Type::Invalid)
    {
        return false;
    }

    m_id   = glCreateShader(type);
    m_type = type;

    if (m_id == EMPTY)
    {
        resetShader();

        return false;
    }

    const GLchar* src = source.c_str();
    GLint size = source.size();

    glShaderSource(m_id, 1, &src, &size);
    glCompileShader(m_id);

    if (!compiled())
    {
        return false;
    }

    return true;
}


//delete & reset
void Shader::deleteShader()
{
    glDeleteShader(m_id);

    resetShader();
}

void Shader::resetShader()
{
    m_id   = EMPTY;
    m_type = Type::Invalid;
}


//checks
bool Shader::valid() const
{
    return m_id == EMPTY;
}

bool Shader::compiled() const
{
    if(!valid())
    {
        return false;
    }

    GLint result;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);

    return result == GL_TRUE;
}


const String& Shader::getInfoLog() const
{
    String& infoLog = Shader::INFO_LOG;

    GLint length;
    GLint returned;

    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
    infoLog.resize(length);

    glGetShaderInfoLog(m_id, length, &returned, infoLog.data());
    infoLog.resize(returned);

    return infoLog;
}


//get & set
GLuint Shader::getId() const
{
    return m_id;
}

Shader::Type Shader::getType() const
{
    return m_type;
}
