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
JavaVM* javaVM;


namespace sf
{
namespace priv
{
void processEvent(ActivityStates* states)
{
    // The caller must ensure states can be safely accessed!

    AInputEvent* event = NULL;

    if (AInputQueue_getEvent(states->inputQueue, &event) >= 0)
    {
        if (AInputQueue_preDispatchEvent(states->inputQueue, event))
            return;

        int32_t handled = 0;

        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
        {

            float x = AMotionEvent_getX(event, 0);
            float y = AMotionEvent_getY(event, 0);

            sf::Event event;
            event.type        = sf::Event::MouseMoved;
            event.mouseMove.x = x;
            event.mouseMove.y = y;
            states->pendingEvents.push_back(event);

            handled = 1;
        }

        AInputQueue_finishEvent(states->inputQueue, event, handled);
    }
}

void processSensorEvents(ActivityStates* states)
{
    // The caller must ensure states can be safely accessed!

    ASensorEvent _event;
    while (ASensorEventQueue_getEvents(states->sensorEventQueue, &_event, 1) > 0)
    {
        sf::Event event;
        event.type       = sf::Event::MouseWheelMoved;
        event.mouseWheel.delta = static_cast<int>(_event.acceleration.x*1000);
        event.mouseWheel.x     = static_cast<int>(_event.acceleration.y*1000);
        event.mouseWheel.y     = static_cast<int>(_event.acceleration.z*1000);
        states->pendingEvents.push_back(event);
    }
}

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

    // Prepare and share the looper to be read later
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    states->looper = looper;

    // Prepare the sensor event queue
    states->sensorEventQueue = ASensorManager_createEventQueue(states->sensorManager, states->looper, 2, NULL, (void*)&sf::priv::processSensorEvents);

    // Get the default configuration
    states->config = AConfiguration_new();
    AConfiguration_fromAssetManager(states->config, states->activity->assetManager);

}

static void terminateMain(ActivityStates* states)
{
    // Protect from concurent access
    sf::Lock lock(states->mutex);

    // The main thread is over, we must explicitly ask the activity to finish
    states->mainOver = true;
    ANativeActivity_finish(states->activity);
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

static void onDestroy(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    // Send an event to warn people the activity is being destroyed
    {
        sf::Lock lock(states->mutex);

        // If the main thread hasn't yet finished, send the event and wait for
        // it to finish.
        if (!states->mainOver)
        {
            sf::Event event;
            event.type = sf::Event::Closed;

            states->pendingEvents.push_back(event);
        }
    }

    // Wait for the main thread to be terminated
    states->mutex.lock();

    while (!states->terminated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states->mutex.lock();
    }

    states->mutex.unlock();

    // Terminate EGL display
    eglCheck(eglTerminate(states->display));

    // Delete our allocated states
    delete states;

    // The application should now terminate
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Update the activity states
    states->window = window;

    // Notify SFML mechanism
    sf::Event event;
    event.type = sf::Event::GainedFocus;
    states->pendingEvents.push_back(event);

    // Wait for the event to be taken into account by SFML
    states->updated = false;
    while(!states->updated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(10));
        states->mutex.lock();
    }
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Update the activity states
    states->window = NULL;

    // Notify SFML mechanism
    sf::Event event;
    event.type = sf::Event::LostFocus;
    states->pendingEvents.push_back(event);

    // Wait for the event to be taken into account by SFML
    states->updated = false;
    while(!states->updated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(10));
        states->mutex.lock();
    }
}

static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
}

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    // Attach the input queue
    {
        sf::Lock lock(states->mutex);

        AInputQueue_attachLooper(queue, states->looper, 1, NULL, (void*)&sf::priv::processEvent);
        states->inputQueue = queue;
    }
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    // Detach the input queue
    {
        sf::Lock lock(states->mutex);

        states->inputQueue = NULL;
        AInputQueue_detachLooper(queue);
    }
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    if (focused)
    {
        // We start monitoring the accelerometer with 60 events per second
        if (states->accelerometerSensor != NULL)
        {
            ASensorEventQueue_enableSensor(states->sensorEventQueue, states->accelerometerSensor);
            ASensorEventQueue_setEventRate(states->sensorEventQueue, states->accelerometerSensor, (1000L/60)*1000);
        }
    }
    else
    {
        // We stop monitoring the accelerometer (it avoids consuming battery)
        if (states->accelerometerSensor != NULL)
        {
            ASensorEventQueue_disableSensor(states->sensorEventQueue, states->accelerometerSensor);
        }
    }
}

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Send an event to warn people about the window move/resize
    sf::Event event;
    event.type = sf::Event::Resized;
    event.size.width = ANativeWindow_getWidth(states->window);
    event.size.height = ANativeWindow_getHeight(states->window);

    states->pendingEvents.push_back(event);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    *outLen = 0;

    return NULL;
}

static void onLowMemory(ANativeActivity* activity)
{
}

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
    // Feed javaVM from openal lib to make it known about our vm instance
    javaVM = activity->vm;

    // Create an activity states (will make us known about events we care)
    sf::priv::ActivityStates* states = NULL;
    states = new sf::priv::ActivityStates;

    // Initialize the states value
    states->activity   = NULL;
    states->window     = NULL;
    states->looper     = NULL;
    states->inputQueue = NULL;
    states->config     = NULL;

    states->sensorManager = ASensorManager_getInstance();
    states->accelerometerSensor = ASensorManager_getDefaultSensor(states->sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    states->sensorEventQueue = NULL;

    states->display = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));

    if (savedState != NULL) {
        states->savedState = malloc(savedStateSize);
        states->savedStateSize = savedStateSize;
        memcpy(states->savedState, savedState, savedStateSize);
    }

    states->mainOver = false;

    states->initialized = false;
    states->terminated = false;

    // Share it across the SFML modules
    sf::priv::getActivityStates(states);

    // These functions will update the activity states and then keep the SFML
    // in the know
    activity->callbacks->onStart   = onStart;
    activity->callbacks->onResume  = onResume;
    activity->callbacks->onPause   = onPause;
    activity->callbacks->onStop    = onStop;
    activity->callbacks->onDestroy = onDestroy;

    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;

    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;

    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onLowMemory = onLowMemory;

    // Share this activity with the callback functions
    states->activity = activity;

    // Initialize the display
    eglCheck(eglInitialize(states->display, NULL, NULL));

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
