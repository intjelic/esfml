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
#include <SFML/Graphics/RenderTargetImpl.hpp>


#if defined(SFML_OPENGL) || defined(SFML_OPENGL_ES)

    #include <SFML/Graphics/OpenGL/RenderTargetImplGL.hpp>
    typedef sf::priv::RenderTargetImplGL RenderTargetImplType;

#elif defined(SFML_DIRECTX)

    #include <SFML/Graphics/DirectX/RenderTargetImplDX.hpp>
    typedef sf::priv::RenderTargetImplDX RenderTargetImplType;

#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderTargetImpl* RenderTargetImpl::create()
{
    return new RenderTargetImplType();
}


////////////////////////////////////////////////////////////
RenderTargetImpl::~RenderTargetImpl()
{
    // Nothing to do
}

} // namespace priv

} // namespace sf
