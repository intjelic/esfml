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

#ifndef SFML_WIDGET_HPP
#define SFML_WIDGET_HPP

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/gui/export.hpp>
#include <sfml/gui/WidgetHandle.hpp>
#include <sfml/graphics/RenderTarget.hpp>
#include <sfml/graphics/RenderTexture.hpp>
#include <sfml/system/Vector2.hpp>


namespace sf
{
namespace priv
{
    class WidgetImpl;
}

class SFML_GUI_API Widget
{
public :

    Widget();
    ~Widget();

    const Vector2i& getPosition() const;
    void setPosition(int, int);
    void setPosition(const Vector2i&);

    const Vector2u& getSize() const;
    void setSize(unsigned int, unsigned int);
    void setSize(const Vector2u&);

    WidgetHandle getWidgetHandle();

protected :

    Widget(priv::WidgetImpl* implementation);

    priv::WidgetImpl* getImplementation();

    virtual void onPaint(RenderTarget& target, const RenderStates& states);
    virtual void onSizeChanged(const Vector2u& newSize, const Vector2u& oldSize);

private :

    priv::WidgetImpl* m_impl;
    Vector2i          m_position;
    Vector2u          m_size;

    friend void paintWidget(Widget* widget, RenderTarget& target, const RenderStates& states);
};

} // namespace sf


#endif // SFML_WIDGET_HPP
