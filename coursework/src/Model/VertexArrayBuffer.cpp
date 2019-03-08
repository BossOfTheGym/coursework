#include "VertexArrayBuffer.h"


VertexArrayBuffer::VertexArrayBuffer(const std::vector<float>& data)
{

}


VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer&& vertexBuffer)
    : m_arrayId(vertexBuffer.m_arrayId)
    , m_bufferId(vertexBuffer.m_bufferId)
{
    vertexBuffer.resetBuffer();
}


VertexArrayBuffer::~VertexArrayBuffer()
{
    deleteBuffer();
}



VertexArrayBuffer& VertexArrayBuffer::operator = (VertexArrayBuffer&& vertexBuffer)
{

}


void VertexArrayBuffer::deleteBuffer()
{

}


void VertexArrayBuffer::resetBuffer()
{
    m_arrayId  = EMPTY;
    m_bufferId = EMPTY;
}


int VertexArrayBuffer::loadData(const std::vector<float>& data)
{

}