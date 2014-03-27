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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Win32/Interface.hpp>
#include <cassert>


namespace
{
    // The shared interface and its reference counter
    IDirect3D9* sharedInterface = NULL;
    unsigned int referenceCount = 0;
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
IDirect3D9* OpenInterface()
{
    if (referenceCount == 0)
        sharedInterface = Direct3DCreate9(D3D_SDK_VERSION);
    referenceCount++;
    return sharedInterface;
}


////////////////////////////////////////////////////////////
void CloseInterface(IDirect3D9* _interface)
{
    assert(_interface == sharedInterface);

    referenceCount--;
    if (referenceCount == 0)
        sharedInterface->Release();
}

} // namespace priv

} // namespace sf
