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

#ifndef SFML_RENDERTARGETIMPLDX_HPP
#define SFML_RENDERTARGETIMPLDX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTargetImpl.hpp>
#include <SFML/Window/Resource.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief DirectX implementation of RenderTargetImpl
///
////////////////////////////////////////////////////////////
class RenderTargetImplDX : public RenderTargetImpl, Resource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTargetImplDX();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderTargetImplDX();

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    /// \param color Fill color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    virtual void clear(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by an array of vertices
    ///
    /// \param vertices    Pointer to the vertices
    /// \param vertexCount Number of vertices in the array
    /// \param type        Type of primitives to draw
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type);

    ////////////////////////////////////////////////////////////
    /// \brief Save the current render states and matrices
    ///
    ////////////////////////////////////////////////////////////
    virtual void pushStates();

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved render states and matrices
    ///
    ////////////////////////////////////////////////////////////
    virtual void popStates();

    ////////////////////////////////////////////////////////////
    /// \brief Reset the internal DirectX states so that the target is ready for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void resetStates();

    ////////////////////////////////////////////////////////////
    /// \brief Apply the current view
    ///
    /// \param view     Current view of the render-target
    /// \param viewport Viewport of the current view, applied to the render target
    /// \param size     Size of the render-target
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyCurrentView(const View& view, IntRect& viewport, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new blending mode
    ///
    /// \param mode Blending mode to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyBlendMode(BlendMode mode);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new transform
    ///
    /// \param transform Transform to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyTransform(const Transform& transform);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new texture
    ///
    /// \param texture Texture to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyTexture(const Texture* texture);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    D3DVIEWPORT9          m_viewport;
    IDirect3DStateBlock9* m_states;
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERTARGETIMPLDX_HPP
