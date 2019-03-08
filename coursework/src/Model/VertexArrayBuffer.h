#pragma once


#include "../Common.h"


class VertexArrayBuffer
{
public:
    static const GLuint EMPTY = 0;


public:
    VertexArrayBuffer(const std::vector<float>& data = {});

    VertexArrayBuffer(const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer);


    ~VertexArrayBuffer();


    VertexArrayBuffer& operator = (const VertexArrayBuffer& vertexBuffer) = delete;

    VertexArrayBuffer& operator = (VertexArrayBuffer&& vertexBuffer) = delete;


    
    int loadData(const std::vector<float>& data);



    void bind();


    void setAttribPointer();

    void setStandartPointers();

    void enableAttribArray();

    void disableAttribPointer();




    GLuint getId() const;


    void deleteBuffer();


private:
    GLuint m_arrayId;
    GLuint m_bufferId;


private:
    void resetBuffer();
};