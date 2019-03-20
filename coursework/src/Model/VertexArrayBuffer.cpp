#include "VertexArrayBuffer.h"


//constructors & destructor
VertexArrayBuffer::VertexArrayBuffer(GLsizei elements, GLsizei size, const float* data)
    : mArrayId(EMPTY)
    , mBufferId(EMPTY)
    , mElements(elements)
{
    if (size > 0)
    {
        glGenVertexArrays(1, &mArrayId);

        if (mArrayId != EMPTY)
        {
            loadData(size, data);
        }
    }
}


VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer)
    : mArrayId(EMPTY)
    , mBufferId(EMPTY)
    , mElements(0)
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
		std::swap(mArrayId, vertexBuffer.mArrayId);
		std::swap(mBufferId, vertexBuffer.mBufferId);
		std::swap(mElements, vertexBuffer.mElements);
	}

    return *this;
}


//get;
GLuint VertexArrayBuffer::getArrayId() const
{
    return mArrayId;
}

GLuint VertexArrayBuffer::getBufferId() const
{
	return mBufferId;
}

GLsizei VertexArrayBuffer::getElements() const
{
    return mElements;
}


//delete & reset
void VertexArrayBuffer::deleteArrayBuffer()
{
    glDeleteBuffers(1, &mBufferId);
    glDeleteVertexArrays(1, &mArrayId);

    resetArrayBuffer();
}


void VertexArrayBuffer::resetArrayBuffer()
{
    mArrayId  = EMPTY;
    mBufferId = EMPTY;

    mElements = static_cast<GLsizei>(0);
}


//core functions
bool VertexArrayBuffer::loadData(GLsizei size, const float* data)
{
    GLint prevArray;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevArray);

    glBindVertexArray(mArrayId);

	glGenBuffers(1, &mBufferId);
    if (mBufferId == EMPTY)
    {
        glDeleteBuffers(1, &mBufferId);

        glBindVertexArray(prevArray);
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);

    glBindVertexArray(prevArray);
    return true;
}


void VertexArrayBuffer::bindArray()
{
    glBindVertexArray(mArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
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