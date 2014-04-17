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
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderWindowImpl.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow()
{
    m_impl = priv::RenderWindowImpl::create();
}


////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
{
    // Don't call the base class constructor because it contains virtual function calls
    create(mode, title, style, settings);

    m_impl = priv::RenderWindowImpl::create();
}


////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(WindowHandle handle, const ContextSettings& settings)
{
    // Don't call the base class constructor because it contains virtual function calls
    create(handle, settings);

    m_impl = priv::RenderWindowImpl::create();
}


////////////////////////////////////////////////////////////
RenderWindow::~RenderWindow()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
bool RenderWindow::activate(bool active)
{
    return setActive(active);
}


////////////////////////////////////////////////////////////
Vector2u RenderWindow::getSize() const
{
    return Window::getSize();
}


////////////////////////////////////////////////////////////
Image RenderWindow::capture() const
{
    Image image;
    if (setActive())
    {
        int width = static_cast<int>(getSize().x);
        int height = static_cast<int>(getSize().y);

        // Let the implementation create the image
        m_impl->capture(image, width, height);
    }

    return image;
}


////////////////////////////////////////////////////////////
void RenderWindow::onCreate()
{
    // Just initialize the render target part
    RenderTarget::initialize();
}


////////////////////////////////////////////////////////////
void RenderWindow::onResize()
{
    // Update the current view (recompute the viewport, which is stored in relative coordinates)
    setView(getView());
}

} // namespace sf
