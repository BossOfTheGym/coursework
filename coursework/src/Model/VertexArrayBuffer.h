#pragma once


#include <Common.h>


class VertexArrayBuffer
{
public:
    static const GLuint EMPTY = 0;


public:
    VertexArrayBuffer(GLsizei elements = static_cast<GLsizei>(0), GLsizei size = 0, const float* data = nullptr);

    VertexArrayBuffer(const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer);


    ~VertexArrayBuffer();


    VertexArrayBuffer& operator = (const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer& operator = (VertexArrayBuffer&& vertexBuffer);



    void bindArray();

    
    void setAttribPointer(GLuint index, GLint size, GLenum element, GLsizei stride, const void* offset);

    void enableAttribArray(GLuint index);

    void disableAttribPointer(GLuint index);



    GLuint getArrayId() const;

	GLuint getBufferId() const;

    GLsizei getElements() const;


    void deleteArrayBuffer();


private:
    void resetArrayBuffer();

    bool loadData(GLsizei size, const float* data);


private:
    GLuint m_arrayId;
    GLuint m_bufferId;

    GLsizei m_elements;
};