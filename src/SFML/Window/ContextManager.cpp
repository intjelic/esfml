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
#include <SFML/Window/ContextManager.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <set>
#include <cstdlib>
#include <cassert>


namespace
{
    // This per-thread variable holds the current context for each thread
    sf::ThreadLocalPtr<sf::priv::GlContext> currentContext(NULL);

    // The hidden, inactive context that will be shared with all other contexts
    ContextType* sharedContext = NULL;

    // Internal contexts
    sf::ThreadLocalPtr<sf::priv::GlContext> internalContext(NULL);
    std::set<sf::priv::GlContext*> internalContexts;
    sf::Mutex internalContextsMutex;

    // Check if the internal context of the current thread is valid
    bool hasInternalContext()
    {
        // The internal context can be null...
        if (!internalContext)
            return false;

        // ... or non-null but deleted from the list of internal contexts
        sf::Lock lock(internalContextsMutex);
        return internalContexts.find(internalContext) != internalContexts.end();
    }

    // Retrieve the internal context for the current thread
    sf::priv::GlContext* getInternalContext()
    {
        if (!hasInternalContext())
        {
            internalContext = sf::priv::ContextManager::create();
            sf::Lock lock(internalContextsMutex);
            internalContexts.insert(internalContext);
        }

        return internalContext;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void ContextManager::globalInit()
{
    // Create the shared context
    sharedContext = new ContextType(NULL);
    sharedContext->initialize();

    // This call makes sure that:
    // - the shared context is inactive (it must never be)
    // - another valid context is activated in the current thread
    sharedContext->setActive(false);
}


////////////////////////////////////////////////////////////
void ContextManager::globalCleanup()
{
    // Destroy the shared context
    delete sharedContext;
    sharedContext = NULL;

    // Destroy the internal contexts
    sf::Lock lock(internalContextsMutex);
    for (std::set<GlContext*>::iterator it = internalContexts.begin(); it != internalContexts.end(); ++it)
        delete *it;
    internalContexts.clear();
}


////////////////////////////////////////////////////////////
void ContextManager::ensureContext()
{
    // If there's no active context on the current thread, activate an internal one
    if (!currentContext)
        getInternalContext()->setActive(true);
}


////////////////////////////////////////////////////////////
GlContext* ContextManager::create()
{
    GlContext* context = new ContextType(sharedContext);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext* ContextManager::create(const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    ensureContext();

    // Create the context
    GlContext* context = new ContextType(sharedContext, settings, owner, bitsPerPixel);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext* ContextManager::create(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    ensureContext();

    // Create the context
    GlContext* context = new ContextType(sharedContext, settings, width, height);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
ContextType* ContextManager::getSharedContext()
{
    return sharedContext;
}


////////////////////////////////////////////////////////////
ThreadLocalPtr<sf::priv::GlContext>& ContextManager::getCurrentContext()
{
    return currentContext;
}


////////////////////////////////////////////////////////////
void ContextManager::setCurrentContext(GlContext* context)
{
    currentContext = context;
}


////////////////////////////////////////////////////////////
priv::GlContext* ContextManager::getInternalContext()
{
    return ::getInternalContext();
}

} // namespace priv

} // namespace sf
