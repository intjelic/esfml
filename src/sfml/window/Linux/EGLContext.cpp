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
#include <sfml/window/Linux/EGLContext.hpp>
#include <sfml/window/Linux/WindowImplX11.hpp>
#include <sfml/window/Linux/Display.hpp>
#include <sfml/window/EGLCheck.hpp>
#include <sfml/system/error.hpp>
#include <sfml/system/Mutex.hpp>
#include <sfml/system/Lock.hpp>

namespace
{
    // EGL resources counter and its mutex
    unsigned int count = 0;
    sf::Mutex mutex;
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////////////////////////
_EGLContext::_EGLContext(_EGLContext* shared) :
m_window     (0),
m_eglContext (EGL_NO_CONTEXT),
m_eglSurface (EGL_NO_SURFACE),
m_ownsWindow (true)
{
	// Make sure the EGL display is initialized
	initializeDisplay();
	
    // Open a connection with the X server
    m_display = OpenDisplay();

    // Create a dummy window (disabled and hidden)
    int screen = DefaultScreen(m_display);
    m_window = XCreateWindow(m_display,
                             RootWindow(m_display, screen),
                             0, 0,
                             1, 1,
                             0,
                             DefaultDepth(m_display, screen),
                             InputOutput,
                             DefaultVisual(m_display, screen),
                             0, NULL);


    // Create the context
    createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
}


////////////////////////////////////////////////////////////////////////////////
_EGLContext::_EGLContext(_EGLContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel) :
m_window    (0),
m_eglContext (EGL_NO_CONTEXT),
m_eglSurface (EGL_NO_SURFACE),
m_ownsWindow(false)
{
	// Make sure the EGL display is initialized
	initializeDisplay();
	
    // Open a connection with the X server
    // (important: must be the same display as the owner window)
    m_display = OpenDisplay();

    // Get the owner window and its device context
    m_window = static_cast< ::Window>(owner->getSystemHandle());

    // Create the context
    if (m_window)
        createContext(shared, bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////////////////////////
_EGLContext::_EGLContext(_EGLContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height) :
m_window    (0),
m_eglContext (EGL_NO_CONTEXT),
m_eglSurface (EGL_NO_SURFACE),
m_ownsWindow(true)
{
    // Make sure the EGL display is initialized
    initializeDisplay();
	
    // Open a connection with the X server
    m_display = OpenDisplay();

    // Create the hidden window
    int screen = DefaultScreen(m_display);
    m_window = XCreateWindow(m_display,
                             RootWindow(m_display, screen),
                             0, 0,
                             width, height,
                             0,
                             DefaultDepth(m_display, screen),
                             InputOutput,
                             DefaultVisual(m_display, screen),
                             0, NULL);
                             
    // Create the context
    createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////////////////////////
_EGLContext::~_EGLContext()
{
    // Destroy the context
    if (m_eglContext)
    {
		EGLContext currentContext = eglCheck(eglGetCurrentContext());
        if (currentContext == m_eglContext)
			eglCheck(eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
        eglCheck(eglDestroyContext(m_eglDisplay, m_eglContext));
    }

    // Destroy the window if we own it
    if (m_window && m_ownsWindow)
    {
        XDestroyWindow(m_display, m_window);
        XFlush(m_display);
    }
    
    // Terminate the EGL display if needed
	terminateDisplay();
}


////////////////////////////////////////////////////////////////////////////////
bool _EGLContext::makeCurrent()
{
    return m_eglContext && eglCheck(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext));
}


////////////////////////////////////////////////////////////////////////////////
void _EGLContext::display()
{
    if (m_window)
		eglCheck(eglSwapBuffers(m_eglDisplay, m_eglSurface));
}


////////////////////////////////////////////////////////////////////////////////
void _EGLContext::setVerticalSyncEnabled(bool enabled)
{
	eglCheck(eglSwapInterval(m_eglDisplay, enabled ? 1 : 0));
}


////////////////////////////////////////////////////////////////////////////////
void _EGLContext::createContext(_EGLContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    m_settings = settings;

    EGLint nConfigs;
    EGLConfig aEGLConfigs[1];

    eglCheck(eglGetConfigs(m_eglDisplay, aEGLConfigs, 1, &nConfigs));

    if (nConfigs == 0) {
        err() << "No available configuration on the display returned" << std::endl;
        return;
    }

    EGLint aEGLContextAttrib[] = { EGL_CONTEXT_CLIENT_VERSION, 1, EGL_NONE };

    EGLContext toShared;

    if (shared)
        toShared = shared->m_eglContext;
    else
        toShared = EGL_NO_CONTEXT;

    m_eglSurface = eglCheck(eglCreateWindowSurface(m_eglDisplay, aEGLConfigs[0], (EGLNativeWindowType)m_window, NULL));

    if (m_eglSurface == EGL_NO_SURFACE) {
        err() << "Failed to create a EGL surface" << std::endl;
        return;
    }

    m_eglContext = eglCheck(eglCreateContext(m_eglDisplay, aEGLConfigs[0], toShared, aEGLContextAttrib));

    if (m_eglContext == EGL_NO_CONTEXT) {
        err() << "Failed to create a EGL context" << std::endl;
        return;
    }
}

void _EGLContext::initializeDisplay()
{
	// Protect from concurrent access
	Lock lock(mutex);

	// If this is the very first context, ...
	if (count == 0)
	{
		// ... retrieve the default EGL display ...
		m_eglDisplay = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));
		
		// ... and initialize it
		eglCheck(eglInitialize(m_eglDisplay, NULL, NULL));		

		count++;
	}
}

void _EGLContext::terminateDisplay()
{
    // Protect from concurrent access
    Lock lock(mutex);

    // Decrement the resources counter
    count--;

    // If there's no more context, ...
    if (count == 0)
    {
		// we can terminate the EGL display ...
		eglCheck(eglTerminate(m_eglDisplay));
		
		// and set it to EGL_NO_DISPLAY
		m_eglDisplay = EGL_NO_DISPLAY;
	}
}

EGLDisplay _EGLContext::m_eglDisplay = EGL_NO_DISPLAY;

} // namespace priv
} // namespace sf
