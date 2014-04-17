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
#include <SFML/Window/Resource.hpp>
#include <SFML/Window/ContextManager.hpp>
#include <SFML/Window/ContextImpl.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>


namespace
{
    // OpenGL resources counter and its mutex
    unsigned int count = 0;
    sf::Mutex mutex;
}


namespace sf
{
////////////////////////////////////////////////////////////
Resource::Resource()
{
    {
        // Protect from concurrent access
        Lock lock(mutex);

        // If this is the very first resource, trigger the global context initialization
        if (count == 0)
            priv::ContextManager::globalInit();

        // Increment the resources counter
        count++;
    }

    // Now make sure that there is an active OpenGL context in the current thread
    priv::ContextManager::ensureContext();
}


////////////////////////////////////////////////////////////
Resource::~Resource()
{
    // Protect from concurrent access
    Lock lock(mutex);

    // Decrement the resources counter
    count--;

    // If there's no more resource alive, we can trigger the global context cleanup
    if (count == 0)
        priv::ContextManager::globalCleanup();
}


////////////////////////////////////////////////////////////
void Resource::ensureContext()
{
    priv::ContextManager::ensureContext();
}


////////////////////////////////////////////////////////////
ContextHandle Resource::getContextHandle() const
{
    return priv::ContextManager::getCurrentContext()->getContextHandle();
}

} // namespace sf