#pragma once

#include <FreeImage/FreeImage.h>


#include "../Common.h"


class Texture2D
{
public:
    static const GLuint EMPTY = 0;


    static void active(GLenum texture);

    static void unbind();


public:
    Texture2D(const String& location = String(""));

    Texture2D(const Texture2D& tex) = delete;

    Texture2D(Texture2D&& tex);


    ~Texture2D();


    Texture2D& operator = (const Texture2D& tex) = delete;

    Texture2D& operator = (Texture2D&& tex);


    void bind();

    void texParameteri(GLenum name, GLint parameter);


    GLuint getId() const;


    void deleteTexture();


private:
    int loadFromLocation(const String& location);

    void resetTexture();


private:
    GLuint m_id;
};