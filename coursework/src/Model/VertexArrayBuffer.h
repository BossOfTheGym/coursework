#pragma once


#include "../Common.h"


class VertexArrayBuffer
{
public:
    static const GLuint EMPTY = 0;

    static const GLint STANDART_SIZE = 3;

    static const GLenum STANDART_ELEMENT = GL_FLOAT;


public:
    VertexArrayBuffer(GLsizei elements = static_cast<GLsizei>(0), const std::vector<float>& data = {});

    VertexArrayBuffer(const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer);


    ~VertexArrayBuffer();


    VertexArrayBuffer& operator = (const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer& operator = (VertexArrayBuffer&& vertexBuffer);



    void bindArray();

    
    void setAttribPointer(GLuint index, GLsizei stride, const void* offset);

    void enableAttribArray(GLuint index);

    void disableAttribPointer(GLuint index);


    bool loadData(const std::vector<float>& data);



    GLuint getArrayId() const;

    GLsizei getElements() const;


    void deleteArrayBuffer();


private:
    void resetArrayBuffer();


private:
    GLuint m_arrayId;
    GLuint m_bufferId;

    GLsizei m_elements;
};