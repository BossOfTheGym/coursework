#include "VertexArrayBuffer.h"


//constructors & destructor
VertexArrayBuffer::VertexArrayBuffer()
	: m_vertexArrayId(EMPTY)
	, m_arrayBufferId(EMPTY)
	, m_count(0)
{}

VertexArrayBuffer::VertexArrayBuffer(GLsizei count, GLsizei size, const double* data)
    : m_vertexArrayId(EMPTY)
    , m_arrayBufferId(EMPTY)
    , m_count(count)
{
    if (size > 0)
    {
        glGenVertexArrays(1, &m_vertexArrayId);

        if (m_vertexArrayId != EMPTY)
        {
            loadData(size, data);
        }
    }
}


VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer)
    : m_vertexArrayId(EMPTY)
    , m_arrayBufferId(EMPTY)
    , m_count(0)
{
	*this = std::move(vertexBuffer);
}


VertexArrayBuffer::~VertexArrayBuffer()
{
    deleteArrayBuffer();
}


//operators
VertexArrayBuffer& VertexArrayBuffer::operator = (VertexArrayBuffer&& vertexBuffer)
{
	if (this != &vertexBuffer)
	{
		std::swap(m_vertexArrayId, vertexBuffer.m_vertexArrayId);
		std::swap(m_arrayBufferId, vertexBuffer.m_arrayBufferId);
		std::swap(m_count, vertexBuffer.m_count);
	}

    return *this;
}


//get;
GLuint VertexArrayBuffer::id() const
{
    return m_vertexArrayId;
}

GLsizei VertexArrayBuffer::primitivesCount() const
{
    return m_count;
}


//delete & reset
void VertexArrayBuffer::deleteArrayBuffer()
{
    glDeleteBuffers(1, &m_arrayBufferId);
    glDeleteVertexArrays(1, &m_vertexArrayId);

    resetArrayBuffer();
}


void VertexArrayBuffer::resetArrayBuffer()
{
    m_vertexArrayId  = EMPTY;
    m_arrayBufferId = EMPTY;

    m_count = static_cast<GLsizei>(0);
}


//core functions
bool VertexArrayBuffer::loadData(GLsizei size, const double* data)
{
    GLint prevArray;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevArray);

    glBindVertexArray(m_vertexArrayId);

	glGenBuffers(1, &m_arrayBufferId);
    if (m_arrayBufferId == EMPTY)
    {
        glDeleteBuffers(1, &m_arrayBufferId);

        glBindVertexArray(prevArray);
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_arrayBufferId);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(double), data, GL_STATIC_DRAW);

    glBindVertexArray(prevArray);
    return true;
}


void VertexArrayBuffer::bind() const
{
    glBindVertexArray(m_vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, m_arrayBufferId);
}


void VertexArrayBuffer::subData(GLintptr offset, GLsizeiptr size, const GLvoid * data)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}


void VertexArrayBuffer::setAttribPointer(GLuint index, GLint size, GLenum element, GLsizei stride, const void* offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, element, GL_FALSE, stride, offset);
}

void VertexArrayBuffer::enableAttribArray(GLuint index)
{
    glEnableVertexAttribArray(index);
}

void VertexArrayBuffer::disableAttribPointer(GLuint index)
{
    glDisableVertexAttribArray(index);
}