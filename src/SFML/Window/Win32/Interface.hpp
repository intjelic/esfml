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

#ifndef SFML_SHAREDINTERFACE_HPP
#define SFML_SHAREDINTERFACE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/DirectX.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Get the shared DirectX interface
///
/// This function increments the reference count of the interface,
/// it must be matched with a call to CloseInterface.
///
/// \return Pointer to the shared interface
///
////////////////////////////////////////////////////////////
IDirect3D9* OpenInterface();

////////////////////////////////////////////////////////////
/// \brief Release a reference to the shared interface
///
/// \param interface Interface to release
///
////////////////////////////////////////////////////////////
void CloseInterface(IDirect3D9* _interface);

} // namespace priv

} // namespace sf


#endif // SFML_SHAREDINTERFACE_HPP
