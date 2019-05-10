#pragma once


#include <Common.h>


class VertexArrayBuffer
{
public:
	VertexArrayBuffer();

    VertexArrayBuffer(GLsizei elements, GLsizei size, const double* data);

    VertexArrayBuffer(const VertexArrayBuffer& vertexBuffer) = delete;
    VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer);

    ~VertexArrayBuffer();

    VertexArrayBuffer& operator = (const VertexArrayBuffer& vertexBuffer) = delete;
    VertexArrayBuffer& operator = (VertexArrayBuffer&& vertexBuffer);



    void bind() const;

	void subData(GLintptr offset, GLsizeiptr size, const GLvoid * data);

    void setAttribPointer(GLuint index, GLint size, GLenum element, GLsizei stride, const void* offset);

    void enableAttribArray(GLuint index);

    void disableAttribPointer(GLuint index);



    GLuint id() const;

    GLsizei primitivesCount() const;


    void deleteArrayBuffer();


private:
    void resetArrayBuffer();

    bool loadData(GLsizei size, const double* data);


private:
    GLuint m_vertexArrayId;
    GLuint m_arrayBufferId;
	GLuint m_elementsBufferId;

	//TODO
	GLenum m_mode;

    GLsizei m_count;
	GLsizei m_elements;
};