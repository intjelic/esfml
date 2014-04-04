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

#ifndef SFML_RENDERTEXTUREIMPLDEFAULT_HPP
#define SFML_RENDERTEXTUREIMPLDEFAULT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTextureImpl.hpp>
#include <SFML/Window/Resource.hpp>
#include <SFML/Window/Context.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Default specialization of RenderTextureImpl
///
////////////////////////////////////////////////////////////
class RenderTextureImplDefault : public RenderTextureImpl, Resource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTextureImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderTextureImplDefault();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Create the render texture implementation
    ///
    /// \param width       Width of the texture to render to
    /// \param height      Height of the texture to render to
    /// \param texture     Texture implementation of the target texture
    /// \param depthBuffer Is a depth buffer requested?
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(unsigned int width, unsigned int height, TextureImpl* texture, bool depthBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render texture for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    virtual bool activate(bool active);

    ////////////////////////////////////////////////////////////
    /// \brief Get the context handle attached to the render texture
    ///
    /// \return Context handle attached to this render texture
    ///
    ////////////////////////////////////////////////////////////
    virtual ContextHandle getContextHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the pixels of the target texture
    ///
    /// \param texture Texture implementation of the target texture
    ///
    ////////////////////////////////////////////////////////////
    virtual void updateTexture(TextureImpl* texture);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Context*     m_context; ///< DirectX context
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERTEXTUREIMPLDEFAULT_HPP
