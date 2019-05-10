#pragma once



class Buffer
{
public:
	static void unbind();


public:
	Buffer();

	Buffer(const Buffer& buffer) = delete;
	Buffer(Buffer&& buffer);

	~Buffer();

	Buffer& operator = (const Buffer& buffer) = delete;
	Buffer& operator = (Buffer&& buffer);


public:
	void bind();

	void genBuffer();

	void bufferData();

	void deleteBuffer();


	//not implemented
	void bufferStorage();
	//not implemented
	GLint getBufferParameteriv();
	//not implemented
	GLint64 getBufferParameter64v();


private:
	GLuint     m_id;
	GLenum     m_target;
	GLsizei    m_size;
	GLenum     m_usage;
	GLbitfield m_flags;
};