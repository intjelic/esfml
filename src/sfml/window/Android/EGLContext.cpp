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
#include <sfml/window/Android/EGLContext.hpp>
#include <sfml/opengl.hpp>
#include <sfml/system/error.hpp>
#include <sfml/system/sleep.hpp>
#include <sfml/system/Mutex.hpp>
#include <sfml/system/Lock.hpp>
#include <sfml/main/activity.hpp>

#include <android/native_window.h>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
_EGLContext::_EGLContext(_EGLContext* shared) :
m_display (EGL_NO_DISPLAY),
m_context (EGL_NO_CONTEXT),
m_surface (EGL_NO_SURFACE)
{
    // Get the activity states and protect it from concurent access
    ActivityStates* states = getActivityStates(NULL);
    sf::Lock lock(states->mutex);

    // Get the intialized EGL display
    m_display = states->display;

    // Create the EGL surface
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_NONE
    };

    EGLint numConfigs;
    EGLConfig config[1];
    eglCheck(eglChooseConfig(m_display, attribs, &config[0], 1, &numConfigs));

    // Note: removing attrib_list crashes the app... idk why
    EGLint attrib_list[] = { EGL_NONE };
    m_surface = eglCheck(eglCreatePbufferSurface(m_display, config[0], attrib_list));

    // Create the context
    createContext(shared, 0, config[0]);
}


////////////////////////////////////////////////////////////
_EGLContext::_EGLContext(_EGLContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel) :
m_display (EGL_NO_DISPLAY),
m_context (EGL_NO_CONTEXT),
m_surface (EGL_NO_SURFACE)
{
    // Get the activity states and protect it from concurent access
    ActivityStates* states = getActivityStates(NULL);
    sf::Lock lock(states->mutex);

    // Get the intialized EGL display
    m_display = states->display;

    // Create the EGL surface
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_NONE
    };

    EGLint numConfigs;
    EGLConfig config[1];
    eglCheck(eglChooseConfig(m_display, attribs, &config[0], 1, &numConfigs));

    // Create the context
    createContext(shared, 0, config[0]);
}


////////////////////////////////////////////////////////////
_EGLContext::_EGLContext(_EGLContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height) :
m_display (EGL_NO_DISPLAY),
m_context (EGL_NO_CONTEXT),
m_surface (EGL_NO_SURFACE)
{
}


////////////////////////////////////////////////////////////
_EGLContext::~_EGLContext()
{
    // Desactive the current context
    EGLContext currentContext = eglCheck(eglGetCurrentContext());

    if (currentContext == m_context)
        eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

    // Destroy context
    eglCheck(eglDestroyContext(m_display, m_context));

    // Destroy surface
    eglCheck(eglDestroySurface(m_display, m_surface));
}


////////////////////////////////////////////////////////////
bool _EGLContext::makeCurrent()
{
    return m_context && eglCheck(eglMakeCurrent(m_display, m_surface, m_surface, m_context));
}


////////////////////////////////////////////////////////////
void _EGLContext::display()
{
    eglCheck(eglSwapBuffers(m_display, m_surface));
}


////////////////////////////////////////////////////////////
void _EGLContext::setVerticalSyncEnabled(bool enabled)
{
    eglCheck(eglSwapInterval(m_display, enabled ? 1 : 0));
}

void _EGLContext::createContext(_EGLContext* shared, unsigned int bitsPerPixel, const EGLConfig settings)
{
    EGLint contextVersion[] = { EGL_CONTEXT_CLIENT_VERSION, 1, EGL_NONE };

    EGLContext toShared;

    if (shared)
        toShared = shared->m_context;
    else
        toShared = EGL_NO_CONTEXT;

    // Create the EGL context
    m_context = eglCheck(eglCreateContext(m_display, settings, toShared, contextVersion));
}


void _EGLContext::createSurface(ANativeWindow* window)
{
    // Create the EGL surface
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_NONE
    };

    EGLint numConfigs;
    EGLConfig config[1];
    eglCheck(eglChooseConfig(m_display, attribs, &config[0], 1, &numConfigs));

    m_surface = eglCheck(eglCreateWindowSurface(m_display, config[0], window, NULL));
}

void _EGLContext::destroySurface()
{
    eglDestroySurface(m_display, m_surface);
}

} // namespace priv

} // namespace sf
