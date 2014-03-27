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

#ifndef SFML_CONTEXTHANDLE_HPP
#define SFML_CONTEXTHANDLE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#if defined(SFML_OPENGL)
    #include <SFML/OpenGL.hpp>
    #if defined(SFML_SYSTEM_IOS)
        #include <OpenGLES/ES1/gl.h>
    #elif defined(SFML_SYSTEM_ANDROID)
        #include <EGL/egl.h>
    #endif
#elif defined(SFML_DIRECTX)
    #include <SFML/DirectX.hpp>
#endif

namespace sf
{
////////////////////////////////////////////////////////////
/// Define a low-level context handle type, specific to
/// each graphics back-end
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_WINDOWS)

    #if defined(SFML_OPENGL)

        // Context handle is HGLR on Windows (if using OpenGL)
        typedef HGLRC ContextHandle;

    #elif defined(SFML_DIRECTX)

        // Context handle is IDirect3DDevice9 on Windows (if using DirectX)
        typedef IDirect3DDevice9* ContextHandle;

    #endif

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    // Context handle is GLXContext on Unix
    typedef GLXContext ContextHandle;

#elif defined(SFML_SYSTEM_MACOS)

    // Context handle is NSOpenGLContextRef on Mac OS X
    typedef NSOpenGLContextRef ContextHandle;

#elif defined(SFML_SYSTEM_IOS)

    // Context handle is EAGLContext on iOS
    typedef EAGLContext ContextHandle;

#else

    // Context handle is EGLContext on platforms uising OpenGL ES
    typedef EGLContext ContextHandle;

#endif

} // namespace sf


#endif // SFML_CONTEXTHANDLE_HPP
