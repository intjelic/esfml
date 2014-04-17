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

#ifndef SFML_RENDERWINDOWIMPLGL_HPP
#define SFML_RENDERWINDOWIMPLGL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderWindowImpl.hpp>
#include <SFML/Window/Resource.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OpenGL implementation of RenderWindowImpl
///
////////////////////////////////////////////////////////////
class RenderWindowImplGL : public RenderWindowImpl, Resource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderWindowImplGL();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderWindowImplGL();

    ////////////////////////////////////////////////////////////
    /// \brief Copy the current contents of the window to an image
    ///
    /// \param image    Image to create
    /// \param width    Width of the window
    /// \param height   Height of the window
    ///
    ////////////////////////////////////////////////////////////
    virtual void capture(Image& image, int width, int height) const;

};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERWINDOWIMPLGL_HPP
