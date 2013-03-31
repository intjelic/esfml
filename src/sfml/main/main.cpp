////////////////////////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
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
////////////////////////////////////////////////////////////////////////////////



#if defined(_WIN32)
////////////////////////////////////////////////////////////////////////////////
// Windows specific : defines the WinMain entry function,
// so that developers can use the standard main function
// even in a Win32 Application project, and keep a portable code
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    return main(__argc, __argv);
}


#elif defined(ANDROID)

#include <sfml/system/sleep.hpp>
#include <sfml/system/Thread.hpp>
#include <sfml/system/Lock.hpp>
#include <sfml/main/activity.hpp>

extern int main(int argc, char *argv[]);

namespace sf
{
namespace priv
{
ActivityStates* getActivityStates(ActivityStates* initializedStates)
{
    static ActivityStates* states = NULL;

    if (!states)
        states = initializedStates;
        
    return states;
}

ActivityStates* retrieveStates(ANativeActivity* activity)
{
    // Hide the ugly cast we find in each callback function
    return (sf::priv::ActivityStates*)activity->instance;
}


static void initializeMain(ActivityStates* states)
{
    // Protect from concurent access
    sf::Lock lock(states->mutex);

    // Nothing to do; will initialize things later
    // ...
}

static void terminateMain(ActivityStates* states)
{
    // Protect from concurent access
    sf::Lock lock(states->mutex);

    // Nothing to do; will terminate things later
    // ...
}

void* main(ActivityStates* states)
{
    // Initialize the thread before giving the hand
    initializeMain(states);
   
    {
        sf::Lock lock(states->mutex);
        
        states->initialized = true;
    }

    sf::sleep(sf::seconds(0.5));
    ::main(0, NULL);

    // Terminate properly the thread and Wait until it's done
    terminateMain(states);

    {
        sf::Lock lock(states->mutex);
        
        states->terminated = true;
    }

    return NULL;
}

} // namespace priv
} // namespace sf

static void onDestroy(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    
    // Wait for the main thread to be terminated
    states->mutex.lock();
    
    while (!states->terminated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states->mutex.lock();
    }
    
    states->mutex.unlock();

    // Delete our allocated states
    delete states;
    
    // The application should now terminate
}

static void onStart(ANativeActivity* activity)
{
}

static void onResume(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
	sf::Lock lock(states->mutex);
	
	// Send an event to warn people the activity has been resumed
    sf::Event event;
    event.type = sf::Event::MouseEntered;

    states->pendingEvents.push_back(event);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    *outLen = 0;
    
    return NULL;
}

static void onPause(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
	sf::Lock lock(states->mutex);
	
	// Send an event to warn people the activity has been paused
    sf::Event event;
    event.type = sf::Event::MouseLeft;

    states->pendingEvents.push_back(event);
}

static void onStop(ANativeActivity* activity)
{
}

static void onConfigurationChanged(ANativeActivity* activity)
{
}

static void onLowMemory(ANativeActivity* activity)
{
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    states->window = window;
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);
    
    states->window = NULL;
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
}

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
    // Create an activity states (will make us known about events we care)
    sf::priv::ActivityStates* states = NULL;
    states = new sf::priv::ActivityStates;

    // Initialize the states value
    states->window = NULL;
    
    if (savedState != NULL) {
        states->savedState = malloc(savedStateSize);
        states->savedStateSize = savedStateSize;
        memcpy(states->savedState, savedState, savedStateSize);
    }
    
    states->initialized = false;
    states->terminated = false;

    // Share it across the SFML modules
    sf::priv::getActivityStates(states);
    
    // These functions will update the activity states and then keep the SFML
    // in the know
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    
    // Share this activity with the callback functions
    states->activity = activity;
    
    // Launch the main thread
    sf::Thread* thread = new sf::Thread(sf::priv::main, states);
    thread->launch();
    
    // Wait for the main thread to be initialized
    states->mutex.lock();
    
    while (!states->initialized)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states->mutex.lock();
    }
    
    states->mutex.unlock();

    // Share this state with the callback functions
    activity->instance = states;
}

#endif
