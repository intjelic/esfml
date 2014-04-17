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

#ifndef SFML_RESOURCE_HPP
#define SFML_RESOURCE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>
#include <SFML/Window/ContextHandle.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Base class for classes that require an OpenGL context
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Resource
{
protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Resource();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Resource();

    ////////////////////////////////////////////////////////////
    /// \brief Make sure that a valid graphics context exists in the current thread
    ///
    ////////////////////////////////////////////////////////////
    static void ensureContext();

    ////////////////////////////////////////////////////////////
    /// \brief Get the hidden and inactive context handle
    ///
    /// The type of the returned handle is sf::ContextHandle,
    /// which is a typedef to the handle type defined by the graphics
    /// back-end used.
    /// You shouldn't need to use this function, unless you have
    /// very specific stuff to implement that SFML doesn't support,
    /// or implement a temporary workaround until a bug is fixed.
    ///
    /// \return Context handle
    ///
    ////////////////////////////////////////////////////////////
    ContextHandle getContextHandle() const;
};

} // namespace sf


#endif // SFML_RESOURCE_HPP

////////////////////////////////////////////////////////////
/// \class sf::Resource
/// \ingroup window
///
/// This class is for internal use only, it must be the base
/// of every class that requires a valid graphics context in
/// order to work.
///
////////////////////////////////////////////////////////////