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
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/ContextManager.hpp>
#include <SFML/OpenGL.hpp>
#include <set>
#include <cstdlib>
#include <cassert>
#ifdef SFML_SYSTEM_IOS
    #include <OpenGLES/ES1/gl.h>
#else
    #include <SFML/Window/glext/glext.h>
#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    // Deactivate the context before killing it, unless we're inside Cleanup()
    if (ContextManager::getSharedContext())
        setActive(false);
}


////////////////////////////////////////////////////////////
void GlContext::initialize()
{
#if defined(SFML_OPENGL)

    // Activate the context
    setActive(true);

    // Retrieve the context version number
    const GLubyte* version = glGetString(GL_VERSION);
    if (version)
    {
        // The beginning of the returned string is "major.minor" (this is standard)
        m_settings.majorVersion = version[0] - '0';
        m_settings.minorVersion = version[2] - '0';
    }
    else
    {
        // Can't get the version number, assume 2.0
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 0;
    }

    // Enable antialiasing if needed
    if (m_settings.antialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);

#elif defined(SFML_DIRECTX)

    // Only version 9.0 of DirectX is supported
    m_settings.majorVersion = 9;
    m_settings.minorVersion = 0;

#endif
}


////////////////////////////////////////////////////////////
const ContextSettings& GlContext::getSettings() const
{
    return m_settings;
}


////////////////////////////////////////////////////////////
bool GlContext::setActive(bool active)
{
    if (active)
    {
        if (this != ContextManager::getCurrentContext())
        {
            // Activate the context
            if (makeCurrent())
            {
                // Set it as the new current context for this thread
                ContextManager::setCurrentContext(this);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            // This context is already the active one on this thread, don't do anything
            return true;
        }
    }
    else
    {
        if (this == ContextManager::getCurrentContext())
        {
            // To deactivate the context, we actually activate another one so that we make
            // sure that there is always an active context for subsequent graphics operations
            return ContextManager::getInternalContext()->setActive(true);
        }
        else
        {
            // This context is not the active one on this thread, don't do anything
            return true;
        }
    }
}


////////////////////////////////////////////////////////////
GlContext::GlContext()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
int GlContext::evaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing)
{
    return std::abs(static_cast<int>(bitsPerPixel               - colorBits))   +
           std::abs(static_cast<int>(settings.depthBits         - depthBits))   +
           std::abs(static_cast<int>(settings.stencilBits       - stencilBits)) +
           std::abs(static_cast<int>(settings.antialiasingLevel - antialiasing));
}

} // namespace priv

} // namespace sf
