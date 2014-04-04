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
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/TextureImpl.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <cassert>
#include <cstring>


namespace
{
    // Thread-safe unique identifier generator,
    // is used for states cache (see RenderTarget)
    sf::Uint64 getUniqueId()
    {
        static sf::Uint64 id = 1; // start at 1, zero is "no texture"
        static sf::Mutex mutex;

        sf::Lock lock(mutex);
        return id++;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Texture::Texture() :
m_impl         (NULL),
m_size         (0, 0),
m_actualSize   (0, 0),
m_isSmooth     (false),
m_isRepeated   (false),
m_cacheId      (getUniqueId())
{

}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy) :
m_impl         (NULL),
m_size         (0, 0),
m_actualSize   (0, 0),
m_isSmooth     (copy.m_isSmooth),
m_isRepeated   (copy.m_isRepeated),
m_cacheId      (getUniqueId())
{
    if (copy.m_impl)
        loadFromImage(copy.copyToImage());
}


////////////////////////////////////////////////////////////
bool Texture::create(unsigned int width, unsigned int height)
{
    // Destroy the previous texture implementation
    close();

    // Check if texture parameters are valid before creating it
    if ((width == 0) || (height == 0))
    {
        err() << "Failed to create texture, invalid size (" << width << "x" << height << ")" << std::endl;
        return false;
    }

    // Compute the internal texture dimensions depending on NPOT textures support
    Vector2u actualSize(priv::TextureImpl::getValidSize(width),
                        priv::TextureImpl::getValidSize(height));

    // Check the maximum texture size
    unsigned int maxSize = priv::TextureImpl::getMaximumSize();
    if ((actualSize.x > maxSize) || (actualSize.y > maxSize))
    {
        err() << "Failed to create texture, its internal size is too high "
              << "(" << actualSize.x << "x" << actualSize.y << ", "
              << "maximum is " << maxSize << "x" << maxSize << ")"
              << std::endl;
        return false;
    }

    // All the validity checks passed, we can store the new texture settings
    m_size.x        = width;
    m_size.y        = height;
    m_actualSize    = actualSize;

    // Create the texture implementation
    m_impl = new priv::TextureImpl;

    if (!m_impl->create(width, height, actualSize, m_isSmooth, m_isRepeated))
    {
        delete m_impl;

        return false;
    }

    m_cacheId = getUniqueId();

    return true;
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
    close();
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFile(const std::string& filename, const IntRect& area)
{
    Image image;
    return image.loadFromFile(filename) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area)
{
    Image image;
    return image.loadFromMemory(data, size) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromStream(InputStream& stream, const IntRect& area)
{
    Image image;
    return image.loadFromStream(stream) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromImage(const Image& image, const IntRect& area)
{
    // Retrieve the image size
    int width = static_cast<int>(image.getSize().x);
    int height = static_cast<int>(image.getSize().y);

    // Load the entire image if the source area is either empty or contains the whole image
    if (area.width == 0 || (area.height == 0) ||
       ((area.left <= 0) && (area.top <= 0) && (area.width >= width) && (area.height >= height)))
    {
        // Load the entire image
        if (create(image.getSize().x, image.getSize().y))
        {
            update(image);

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Load a sub-area of the image

        // Adjust the rectangle to the size of the image
        IntRect rectangle = area;
        if (rectangle.left   < 0) rectangle.left = 0;
        if (rectangle.top    < 0) rectangle.top  = 0;
        if (rectangle.left + rectangle.width > width)  rectangle.width  = width - rectangle.left;
        if (rectangle.top + rectangle.height > height) rectangle.height = height - rectangle.top;

        // Create the texture and upload the pixels
        if (create(rectangle.width, rectangle.height))
        {
            return m_impl->loadFromImage(image, rectangle);
        }
        else
        {
            return false;
        }
    }
}


////////////////////////////////////////////////////////////
Vector2u Texture::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
Image Texture::copyToImage() const
{
    // Easy case: empty texture
    if (!m_impl)
        return Image();

    // Copy from texture to our image
    Image image;
    m_impl->copyToImage(m_size, m_actualSize, image);

    return image;
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels)
{
    // Update the whole texture
    update(pixels, m_size.x, m_size.y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    assert(x + width <= m_size.x);
    assert(y + height <= m_size.y);

    if (pixels && m_impl)
    {
        m_impl->update(pixels, width, height, x, y);

        m_cacheId = getUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image)
{
    // Update the whole texture
    update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image, unsigned int x, unsigned int y)
{
    update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, x, y);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window)
{
    update(window, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window, unsigned int x, unsigned int y)
{
    assert(x + window.getSize().x <= m_size.x);
    assert(y + window.getSize().y <= m_size.y);


    if (m_impl && window.setActive(true))
    {
        m_impl->update(window, x, y);

        m_cacheId = getUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    if (smooth != m_isSmooth)
    {
        m_isSmooth = smooth;

        m_impl->setSmooth(m_isSmooth);
    }
}


////////////////////////////////////////////////////////////
bool Texture::isSmooth() const
{
    return m_isSmooth;
}


////////////////////////////////////////////////////////////
void Texture::setRepeated(bool repeated)
{
    if (repeated != m_isRepeated)
    {
        m_isRepeated = repeated;

        m_impl->setRepeated(m_isRepeated);
    }
}


////////////////////////////////////////////////////////////
bool Texture::isRepeated() const
{
    return m_isRepeated;
}


////////////////////////////////////////////////////////////
void Texture::bind(const Texture* texture, CoordinateType coordinateType)
{
    if (texture && texture->m_impl)
        priv::TextureImpl::bind(texture->m_impl, coordinateType, texture->m_size, texture->m_actualSize);
}


////////////////////////////////////////////////////////////
unsigned int Texture::getMaximumSize()
{
    return priv::TextureImpl::getMaximumSize();
}


////////////////////////////////////////////////////////////
void Texture::close()
{
    if (m_impl)
    {
        delete m_impl;
        m_impl = NULL;
    }

    m_size = Vector2u(0, 0);
    m_actualSize = Vector2u(0, 0);
    m_isSmooth = false;
    m_isRepeated = false;
    m_cacheId = 0;
}


////////////////////////////////////////////////////////////
Texture& Texture::operator =(const Texture& right)
{
    Texture temp(right);

    std::swap(m_size,       temp.m_size);
    std::swap(m_actualSize, temp.m_actualSize);
    std::swap(m_impl,       temp.m_impl);
    std::swap(m_isSmooth,   temp.m_isSmooth);
    std::swap(m_isRepeated, temp.m_isRepeated);
    m_cacheId = getUniqueId();

    return *this;
}


////////////////////////////////////////////////////////////
unsigned int Texture::getValidSize(unsigned int size)
{
    return priv::TextureImpl::getValidSize(size);
}

} // namespace sf
