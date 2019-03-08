#include "Texture2D.h"


//statics
void Texture2D::active(GLenum texture)
{
    glActiveTexture(texture);
}

void Texture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


//constructors & destructor
Texture2D::Texture2D(const String& location)
{
    GLint prevTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);

    glGenTextures(1, &m_id);
    if (m_id == EMPTY || !loadFromLocation(location))
    {
        deleteTexture();
    }
   
    glBindTexture(GL_TEXTURE_2D, prevTexture);
}

Texture2D::Texture2D(Texture2D&& texture) : m_id(texture.m_id)
{
    texture.resetTexture();
}


Texture2D::~Texture2D()
{
    deleteTexture();
}


//operators
Texture2D& Texture2D::operator = (Texture2D&& texture)
{
    deleteTexture();

    m_id = texture.m_id;

    texture.resetTexture();

    return *this;
}


//core functions
void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}


void Texture2D::texParameteri(GLenum name, GLint parameter)
{
    glTexParameteri(GL_TEXTURE_2D, name, parameter);
}


//get & set
GLuint Texture2D::getId() const
{
    return m_id;
}


//delete
void Texture2D::deleteTexture()
{
    glDeleteTextures(1, &m_id);

    resetTexture();
}


//private
int Texture2D::loadFromLocation(const String& location)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(location.c_str(), 0);
    if (format == -1)
    {
        std::cout << "Could not find image" << std::endl;
        return false;
    }
    if (format == FIF_UNKNOWN)
    {
        std::cout << "Couldn't determine file format - attempting to get from file extension..." << std::endl;

        format = FreeImage_GetFIFFromFilename(location.c_str());

        if (!FreeImage_FIFSupportsReading(format))
        {
            std::cout << "Detected image format cannot be read!" << std::endl;
            return false;
        }
    }

    FIBITMAP* bitmap = FreeImage_Load(format, location.c_str());
    int bitsPerPixel = FreeImage_GetBPP(bitmap);

    FIBITMAP* bitmap32;
    if (bitsPerPixel == 32)
    {
        std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << std::endl;
        bitmap32 = bitmap;
    }
    else
    {
        std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << std::endl;
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }

    int imageWidth = FreeImage_GetWidth(bitmap32);
    int imageHeight = FreeImage_GetHeight(bitmap32);

    GLubyte* textureData = FreeImage_GetBits(bitmap32);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        imageWidth,
        imageHeight,
        0,
        GL_BGRA,
        GL_UNSIGNED_BYTE,
        textureData
    );

    FreeImage_Unload(bitmap32);

    if (bitsPerPixel != 32)
    {
        FreeImage_Unload(bitmap);
    }

    return true;
}


void Texture2D::resetTexture()
{
    m_id = EMPTY;
}