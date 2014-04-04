////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/TextureImpl.hpp>
#include <SFML/Graphics/OpenGL/TextureSaver.hpp>
#include <SFML/Window/Window.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
TextureImpl::TextureImpl() :
m_texture      (0),
m_pixelsFlipped(false)
{

}

////////////////////////////////////////////////////////////
TextureImpl::~TextureImpl()
{
    // Destroy the OpenGL texture
    if (m_texture)
    {
        ensureContext();

        GLuint texture = static_cast<GLuint>(m_texture);
        glCheck(glDeleteTextures(1, &texture));
    }
}

////////////////////////////////////////////////////////////
bool TextureImpl::create(unsigned int width, unsigned int height, const Vector2u& actualSize, bool smooth, bool repeated)
{
    ensureContext();

    // Create the OpenGL texture if it doesn't exist yet
    if (!m_texture)
    {
        GLuint texture;
        glCheck(glGenTextures(1, &texture));
        m_texture = static_cast<unsigned int>(texture);
    }

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Initialize the texture
    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, actualSize.x, actualSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));

    return true;
}

////////////////////////////////////////////////////////////
bool TextureImpl::loadFromImage(const Image& image, const IntRect& rectangle)
{
    // Retrieve the image size
    int width = static_cast<int>(image.getSize().x);
    int height = static_cast<int>(image.getSize().y);

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Copy the pixels to the texture, row by row
    const Uint8* pixels = image.getPixelsPtr() + 4 * (rectangle.left + (width * rectangle.top));
    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    for (int i = 0; i < rectangle.height; ++i)
    {
        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rectangle.width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
        pixels += 4 * width;
    }

    // Force an OpenGL flush, so that the texture will appear updated
    // in all contexts immediately (solves problems in multi-threaded apps)
    glCheck(glFlush());

    return true;
}

////////////////////////////////////////////////////////////
void TextureImpl::copyToImage(const Vector2u& size, const Vector2u& actualSize, Image& image) const
{
    ensureContext();

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Create an array of pixels
    std::vector<Uint8> pixels(size.x * size.y * 4);

#ifdef SFML_OPENGL_ES

    // OpenGL ES doesn't have the glGetTexImage function, the only way to read
    // from a texture is to bind it to a FBO and use glReadPixels
    GLuint frameBuffer = 0;
    glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
    if (frameBuffer)
    {
        GLint previousFrameBuffer;
        glCheck(glGetIntegerv(GLEXT_GL_FRAMEBUFFER_BINDING, &previousFrameBuffer));

        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, frameBuffer));
        glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0));
        glCheck(glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
        glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));

        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, previousFrameBuffer));
    }

#else

    if ((size == actualSize) && !m_pixelsFlipped)
    {
        // Texture is not padded nor flipped, we can use a direct copy
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
    }
    else
    {
        // Texture is either padded or flipped, we have to use a slower algorithm

        // All the pixels will first be copied to a temporary array
        std::vector<Uint8> allPixels(actualSize.x * actualSize.y * 4);
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]));

        // Then we copy the useful pixels from the temporary array to the final one
        const Uint8* src = &allPixels[0];
        Uint8* dst = &pixels[0];
        int srcPitch = actualSize.x * 4;
        int dstPitch = size.x * 4;

        // Handle the case where source pixels are flipped vertically
        if (m_pixelsFlipped)
        {
            src += srcPitch * (size.y - 1);
            srcPitch = -srcPitch;
        }

        for (unsigned int i = 0; i < size.y; ++i)
        {
            std::memcpy(dst, src, dstPitch);
            src += srcPitch;
            dst += dstPitch;
        }
    }

#endif // SFML_OPENGL_ES

    // Create the image
    image.create(size.x, size.y, &pixels[0]);
}

////////////////////////////////////////////////////////////
void TextureImpl::update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    if (m_texture)
    {
        ensureContext();

        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy pixels from the given array to the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
        m_pixelsFlipped = false;

        // Force an OpenGL flush, so that the texture will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glCheck(glFlush());
    }
}

////////////////////////////////////////////////////////////
void TextureImpl::update(const Window& window, unsigned int x, unsigned int y)
{
    if (m_texture)
    {
        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy pixels from the back-buffer to the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 0, 0, window.getSize().x, window.getSize().y));
        m_pixelsFlipped = true;

        // Force an OpenGL flush, so that the texture will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glCheck(glFlush());
    }
}


////////////////////////////////////////////////////////////
void TextureImpl::setSmooth(bool smooth)
{
    if (m_texture)
    {
        ensureContext();

        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
    }
}

////////////////////////////////////////////////////////////
void TextureImpl::setRepeated(bool repeated)
{
    if (m_texture)
    {
        ensureContext();

        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    }
}

////////////////////////////////////////////////////////////
void TextureImpl::bind(const TextureImpl* texture, Texture::CoordinateType coordinateType, const Vector2u& size, const Vector2u& actualSize)
{
    ensureContext();

    if (texture->m_texture)
    {
        // Bind the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, texture->m_texture));

        // Check if we need to define a special texture matrix
        if ((coordinateType == Texture::Pixels) || texture->m_pixelsFlipped)
        {
            GLfloat matrix[16] = {1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f};

            // If non-normalized coordinates (= pixels) are requested, we need to
            // setup scale factors that convert the range [0 .. size] to [0 .. 1]
            if (coordinateType == Texture::Pixels)
            {
                matrix[0] = 1.f / actualSize.x;
                matrix[5] = 1.f / actualSize.y;
            }

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                matrix[5] = -matrix[5];
                matrix[13] = static_cast<float>(size.y) / actualSize.y;
            }

            // Load the matrix
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glLoadMatrixf(matrix));

            // Go back to model-view mode (sf::RenderTarget relies on it)
            glCheck(glMatrixMode(GL_MODELVIEW));
        }
    }
    else
    {
        // Bind no texture
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));

        // Reset the texture matrix
        glCheck(glMatrixMode(GL_TEXTURE));
        glCheck(glLoadIdentity());

        // Go back to model-view mode (sf::RenderTarget relies on it)
        glCheck(glMatrixMode(GL_MODELVIEW));
    }
}


////////////////////////////////////////////////////////////
unsigned int TextureImpl::getMaximumSize()
{
    ensureContext();

    GLint size;
    glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));

    return static_cast<unsigned int>(size);
}

////////////////////////////////////////////////////////////
unsigned int TextureImpl::getValidSize(unsigned int size)
{
    ensureContext();

    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    if (GLEXT_texture_non_power_of_two)
    {
        // If hardware supports NPOT textures, then just return the unmodified size
        return size;
    }
    else
    {
        // If hardware doesn't support NPOT textures, we calculate the nearest power of two
        unsigned int powerOfTwo = 1;
        while (powerOfTwo < size)
            powerOfTwo *= 2;

        return powerOfTwo;
    }
}

} // namespace priv

} // namespace sf

