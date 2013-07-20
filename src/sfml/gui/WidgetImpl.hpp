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

#ifndef SFML_WIDGETIMPL_HPP
#define SFML_WIDGETIMPL_HPP

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/gui/WidgetHandle.hpp>
#include <sfml/graphics/RenderTarget.hpp>
#include <sfml/graphics/RenderStates.hpp>
#include <sfml/system/Vector2.hpp>


namespace sf
{
namespace priv
{
class WidgetImpl
{
public :

    virtual void size(const Vector2u& newSize, const Vector2u& oldSize)=0;
    virtual void paint(RenderTarget& target, const RenderStates& states)=0;

    virtual WidgetHandle getWidgetHandle()=0;

};

} // namespace priv
} // namespace sf


#endif // SFML_WIDGETIMPL_HPP
