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

#ifndef SFML_WINDOWIMPLANDROID_HPP
#define SFML_WINDOWIMPLANDROID_HPP

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/window/Event.hpp>
#include <sfml/window/WindowImpl.hpp>
#include <sfml/window/Android/EGLContext.hpp>

namespace sf
{
namespace priv
{
class WindowImplAndroid : public WindowImpl
{
public :

    WindowImplAndroid(WindowHandle handle);
    WindowImplAndroid(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings);
    ~WindowImplAndroid();

    virtual WindowHandle getSystemHandle() const;

    virtual Vector2i getPosition() const;
    virtual void setPosition(const Vector2i& position);

    virtual Vector2u getSize() const;
    virtual void setSize(const Vector2u& size);

    virtual void setTitle(const String& title);

    virtual void setIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    virtual void setVisible(bool visible);

    virtual void setMouseCursorVisible(bool visible);
    virtual void setKeyRepeatEnabled(bool enabled);

    _EGLContext* m_context;

protected :
    virtual void processEvents();

private:
    // Cache the width and height, as calls to ANativeWindow_getWidth/Height can be slow
    int m_width;
    int m_height;
};

} // namespace priv

} // namespace sf


#endif // SFML_WINDOWIMPLANDROID_HPP
