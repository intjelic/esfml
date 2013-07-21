////////////////////////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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


#include <android/native_activity.h>
#include <android/configuration.h>
#include <android/sensor.h>
#include <sfml/window/EGLCheck.hpp>
#include <sfml/window/Event.hpp>
#include <sfml/system/Mutex.hpp>
#include <vector>

namespace sf
{
namespace priv
{
struct ActivityStates
{
    ANativeActivity* activity;
    ANativeWindow* window;

    ALooper*        looper;
    AInputQueue*    inputQueue;
    AConfiguration* config;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    EGLDisplay display;

    void* savedState;
    size_t savedStateSize;

    sf::Mutex mutex;

    std::vector<sf::Event> pendingEvents;

    bool mainOver;

    bool initialized;
    bool terminated;

    bool updated;
};

ActivityStates* getActivityStates(ActivityStates* initializedStates=NULL);
} // namespace priv
} // namespace sf
