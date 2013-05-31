////////////////////////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
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
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/gui/Widget.hpp>


namespace sf
{
////////////////////////////////////////////////////////////////////////////////
Widget::Widget() :
m_position (0, 0),
m_size     (0, 0)
{
}


////////////////////////////////////////////////////////////////////////////////
Widget::~Widget()
{
}


////////////////////////////////////////////////////////////////////////////////
const sf::Vector2i& Widget::getPosition() const
{
    return m_position;
}


////////////////////////////////////////////////////////////////////////////////
void Widget::setPosition(int x, int y)
{
    m_position.x = x;
    m_position.y = y;
}


////////////////////////////////////////////////////////////////////////////////
void Widget::setPosition(const sf::Vector2i& position)
{
    setPosition(position.x, position.y);
}


////////////////////////////////////////////////////////////////////////////////
const sf::Vector2u& Widget::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////////////////////////
void Widget::setSize(unsigned int width, unsigned int height)
{
    onSizeChanged(Vector2u(width, height), m_size);

    m_size.x = width;
    m_size.y = height;
}


////////////////////////////////////////////////////////////////////////////////
void Widget::setSize(const sf::Vector2u& size)
{
    setSize(size.x, size.y);
}


////////////////////////////////////////////////////////////////////////////////
void Widget::onSizeChanged(const sf::Vector2u& newSize, const sf::Vector2u& oldSize)
{
}

} // namespace sf
