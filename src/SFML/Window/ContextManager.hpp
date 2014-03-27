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

#ifndef SFML_CONTEXTMANAGER_HPP
#define SFML_CONTEXTMANAGER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/ThreadLocalPtr.hpp>


#if !defined(SFML_OPENGL_ES)

    #if defined(SFML_SYSTEM_WINDOWS)

        #if defined(SFML_OPENGL)

            #include <SFML/Window/Win32/WglContext.hpp>
            typedef sf::priv::WglContext ContextType;

        #elif defined(SFML_DIRECTX)

            #include <SFML/Window/Win32/DxContext.hpp>
            typedef sf::priv::DxContext ContextType;

        #endif

    #elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

        #include <SFML/Window/Unix/GlxContext.hpp>
        typedef sf::priv::GlxContext ContextType;

    #elif defined(SFML_SYSTEM_MACOS)

        #include <SFML/Window/OSX/SFContext.hpp>
        typedef sf::priv::SFContext ContextType;

    #endif

#else

    #if defined(SFML_SYSTEM_IOS)

        #include <SFML/Window/iOS/EaglContext.hpp>
        typedef sf::priv::EaglContext ContextType;

    #else

        #include <SFML/Window/EglContext.hpp>
        typedef sf::priv::EglContext ContextType;

    #endif

#endif


namespace sf
{
namespace priv
{
class WindowImpl;

////////////////////////////////////////////////////////////
/// \brief Global context manager
///
////////////////////////////////////////////////////////////
class ContextManager : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global initialization
    ///
    /// This function is called once, before the very first OpenGL
    /// resource is created. It makes sure that everything is ready
    /// for contexts to work properly.
    /// Note: this function doesn't need to be thread-safe, as it
    /// can be called only once.
    ///
    ////////////////////////////////////////////////////////////
    static void globalInit();

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global cleanup
    ///
    /// This function is called after the very last OpenGL resource
    /// is destroyed. It makes sure that everything that was
    /// created by initialize() is properly released.
    /// Note: this function doesn't need to be thread-safe, as it
    /// can be called only once.
    ///
    ////////////////////////////////////////////////////////////
    static void globalCleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Ensures that an OpenGL context is active in the current thread
    ///
    ////////////////////////////////////////////////////////////
    static void ensureContext();

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context, not associated to a window
    ///
    /// This function automatically chooses the specialized class
    /// to use according to the OS.
    ///
    /// \return Pointer to the created context (don't forget to delete it)
    ///
    ////////////////////////////////////////////////////////////
    static ContextImpl* create();

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context attached to a window
    ///
    /// This function automatically chooses the specialized class
    /// to use according to the OS.
    ///
    /// \param settings     Creation parameters
    /// \param owner        Pointer to the owner window
    /// \param bitsPerPixel Pixel depth (in bits per pixel)
    ///
    /// \return Pointer to the created context
    ///
    ////////////////////////////////////////////////////////////
    static ContextImpl* create(const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context that embeds its own rendering target
    ///
    /// This function automatically chooses the specialized class
    /// to use according to the OS.
    ///
    /// \param settings Creation parameters
    /// \param width    Back buffer width
    /// \param height   Back buffer height
    ///
    /// \return Pointer to the created context
    ///
    ////////////////////////////////////////////////////////////
    static ContextImpl* create(const ContextSettings& settings, unsigned int width, unsigned int height);
    
private:

    friend class ContextImpl;
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the (hidden and inactive) shared context
    ///
    ////////////////////////////////////////////////////////////
    static ContextType* getSharedContext();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current context in this thread
    ///
    ////////////////////////////////////////////////////////////
	static ThreadLocalPtr<ContextImpl>& getCurrentContext();

    ////////////////////////////////////////////////////////////
    /// \brief Set the current context for this thread
    ///
    ////////////////////////////////////////////////////////////
	static void setCurrentContext(ContextImpl* context);

    ////////////////////////////////////////////////////////////
    /// \brief Get the internal context for the current thread
    ///
    ////////////////////////////////////////////////////////////
	static ContextImpl* getInternalContext();
};

} // namespace priv

} // namespace sf


#endif // SFML_CONTEXTMANAGER_HPP
