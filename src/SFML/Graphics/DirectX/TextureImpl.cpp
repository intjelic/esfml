////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013-2014 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Graphics/DirectX/TextureSaver.hpp>
#include <SFML/Window/Window.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
TextureImpl::TextureImpl() :
m_texture      (NULL),
m_pixelsFlipped(false)
{

}

////////////////////////////////////////////////////////////
TextureImpl::~TextureImpl()
{
}

////////////////////////////////////////////////////////////
bool TextureImpl::create(unsigned int width, unsigned int height, const Vector2u& actualSize, bool smooth, bool repeated)
{
    // To implement
    return true;
}

////////////////////////////////////////////////////////////
bool TextureImpl::loadFromImage(const Image& image, const IntRect& area)
{
    // To implement
    return true;
}

////////////////////////////////////////////////////////////
void TextureImpl::copyToImage(const Vector2u& size, const Vector2u& actualSize, Image& image) const
{
    // To implement
}

////////////////////////////////////////////////////////////
void TextureImpl::update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    // To implement
}

////////////////////////////////////////////////////////////
void TextureImpl::update(const Window& window, unsigned int x, unsigned int y)
{
    // To implement
}

////////////////////////////////////////////////////////////
void TextureImpl::setSmooth(bool smooth)
{
    // To implement
}

////////////////////////////////////////////////////////////
void TextureImpl::setRepeated(bool repeated)
{
    // To implement
}

////////////////////////////////////////////////////////////
void TextureImpl::bind(const TextureImpl* texture, Texture::CoordinateType coordinateType, const Vector2u& size, const Vector2u& actualSize)
{
    // To implement
}

////////////////////////////////////////////////////////////
unsigned int TextureImpl::getMaximumSize()
{
    // To implement
    return 8192;
}


////////////////////////////////////////////////////////////
unsigned int TextureImpl::getValidSize(unsigned int size)
{
    // To implement
    return size;
}

} // namespace priv

} // namespace sf