//#pragma once
//
//#include <Common.h>
//#include <Entity/IObject.h>
//
//
//class IShader : public IObject
//{
//public:
//	enum Type : GLenum
//	{
//		Vertex         = GL_VERTEX_SHADER
//		, TessControl    = GL_TESS_CONTROL_SHADER
//		, TessEvaluation = GL_TESS_EVALUATION_SHADER
//		, Geometry       = GL_GEOMETRY_SHADER
//		, Fragment       = GL_FRAGMENT_SHADER
//		, Invalid = 0
//	};
//
//public:
//	virtual GLuint id() const = 0;
//
//	virtual Type type() const = 0;
//
//
//	virtual bool valid() const = 0;
//
//	virtual bool compiled() const = 0;
//
//
//	virtual const String& infoLog() const = 0;
//};
//
//
//class IShaderProgram : public IObject
//{
//public:
//
//};