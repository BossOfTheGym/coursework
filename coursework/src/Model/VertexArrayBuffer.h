#pragma once


#include <Common.h>


class VertexArrayBuffer
{
public:
    static const GLuint EMPTY = 0;


public:
	VertexArrayBuffer();

    VertexArrayBuffer(GLsizei elements, GLsizei size, const float* data);

    VertexArrayBuffer(const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer);


    ~VertexArrayBuffer();


    VertexArrayBuffer& operator = (const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer& operator = (VertexArrayBuffer&& vertexBuffer);



    void bindArray() const;

	void subData(GLintptr offset, GLsizeiptr size, const GLvoid * data);

    void setAttribPointer(GLuint index, GLint size, GLenum element, GLsizei stride, const void* offset);

    void enableAttribArray(GLuint index);

    void disableAttribPointer(GLuint index);



    GLuint arrayId() const;

	GLuint bufferId() const;

    GLsizei elements() const;


    void deleteArrayBuffer();


private:
    void resetArrayBuffer();

    bool loadData(GLsizei size, const float* data);


private:
    GLuint mArrayId;
    GLuint mBufferId;

    GLsizei mElements;
};