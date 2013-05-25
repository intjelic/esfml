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

#ifndef SFML_EGLCONTEXT_HPP
#define SFML_EGLCONTEXT_HPP

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/window/VideoMode.hpp>
#include <sfml/window/ContextSettings.hpp>
#include <sfml/window/EGLCheck.hpp>
#include <sfml/window/GlContext.hpp>
#include <sfml/opengl.hpp>


namespace sf
{
namespace priv
{
class _EGLContext : public GlContext
{
public :

    _EGLContext(_EGLContext* shared);
    _EGLContext(_EGLContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel);
    _EGLContext(_EGLContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height);
    ~_EGLContext();

    virtual bool makeCurrent();
    virtual void display();
    virtual void setVerticalSyncEnabled(bool enabled);

    void createContext(_EGLContext* shared, unsigned int bitsPerPixel, const EGLConfig settings);

    void createSurface(ANativeWindow* window);
    void destroySurface();

private :
    EGLDisplay  m_display;
    EGLContext  m_context;
    EGLSurface  m_surface;

};

} // namespace priv

} // namespace sf


#endif // SFML_EGLCONTEXT_HPP
