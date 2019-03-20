#include "VertexArrayBuffer.h"


//constructors & destructor
VertexArrayBuffer::VertexArrayBuffer(GLsizei elements, GLsizei size, const float* data)
    : m_arrayId(EMPTY)
    , m_bufferId(EMPTY)
    , m_elements(elements)
{
    if (size > 0)
    {
        glGenVertexArrays(1, &m_arrayId);

        if (m_arrayId != EMPTY)
        {
            loadData(size, data);
        }
    }
}


VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer)
    : m_arrayId(vertexBuffer.m_arrayId)
    , m_bufferId(vertexBuffer.m_bufferId)
    , m_elements(vertexBuffer.m_elements)
{
    vertexBuffer.resetArrayBuffer();
}


VertexArrayBuffer::~VertexArrayBuffer()
{
    deleteArrayBuffer();
}


//operators
VertexArrayBuffer& VertexArrayBuffer::operator = (VertexArrayBuffer&& vertexBuffer)
{
    deleteArrayBuffer();

    m_arrayId = vertexBuffer.m_arrayId;
    m_bufferId = vertexBuffer.m_bufferId;
    m_elements = vertexBuffer.m_elements;

    vertexBuffer.resetArrayBuffer();

    return *this;
}


//get;
GLuint VertexArrayBuffer::getArrayId() const
{
    return m_arrayId;
}

GLuint VertexArrayBuffer::getBufferId() const
{
	return m_bufferId;
}

GLsizei VertexArrayBuffer::getElements() const
{
    return m_elements;
}


//delete & reset
void VertexArrayBuffer::deleteArrayBuffer()
{
    glDeleteBuffers(1, &m_bufferId);
    glDeleteVertexArrays(1, &m_arrayId);

    resetArrayBuffer();
}


void VertexArrayBuffer::resetArrayBuffer()
{
    m_arrayId  = EMPTY;
    m_bufferId = EMPTY;

    m_elements = static_cast<GLsizei>(0);
}


//core functions
bool VertexArrayBuffer::loadData(GLsizei size, const float* data)
{
    GLint prevArray;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevArray);

    glBindVertexArray(m_arrayId);

	glGenBuffers(1, &m_bufferId);
    if (m_bufferId == EMPTY)
    {
        glDeleteBuffers(1, &m_bufferId);

        glBindVertexArray(prevArray);
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);

    glBindVertexArray(prevArray);
    return true;
}


void VertexArrayBuffer::bindArray()
{
    glBindVertexArray(m_arrayId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
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