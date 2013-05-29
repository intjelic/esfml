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
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
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


////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <sfml/window/Android/WindowImplAndroid.hpp>
#include <sfml/window/Event.hpp>
#include <sfml/system/Lock.hpp>
#include <sfml/main/activity.hpp>
#include <jni.h>

////////////////////////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////////////////////////
namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(WindowHandle handle) :
m_context (NULL)
{
}


////////////////////////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(VideoMode mode, const std::string& title, unsigned long style) :
m_context (NULL)
{
}


////////////////////////////////////////////////////////////////////////////////
WindowImplAndroid::~WindowImplAndroid()
{
}


////////////////////////////////////////////////////////////////////////////////
WindowHandle WindowImplAndroid::getSystemHandle() const
{
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::processEvents()
{
    ActivityStates* states = getActivityStates(NULL);
    sf::Lock lock(states->mutex);

    int ident, events;
    void (*addEventToQueue)(ActivityStates*);

    while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&addEventToQueue)) >= 0)
    {
        addEventToQueue(states);
    }

    while (!states->pendingEvents.empty())
    {
        sf::Event tempEvent = states->pendingEvents.back();
        states->pendingEvents.pop_back();

        if (tempEvent.type == sf::Event::Resized)
        {
            m_width = tempEvent.size.width;
            m_height = tempEvent.size.height;
        }
        else if (tempEvent.type == Event::GainedFocus)
        {
            m_context->createSurface(states->window);
            states->updated = true;
        }
        else if (tempEvent.type == Event::LostFocus)
        {
            m_context->destroySurface();
            states->updated = true;
        }

        pushEvent(tempEvent);
    }
}


////////////////////////////////////////////////////////////////////////////////
Vector2i WindowImplAndroid::getPosition() const
{
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setPosition(const Vector2i& position)
{
}


////////////////////////////////////////////////////////////////////////////////
Vector2u WindowImplAndroid::getSize() const
{
    return Vector2u(static_cast<unsigned int>(m_width), static_cast<unsigned int>(m_height));
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setSize(const Vector2u& size)
{
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setTitle(const String& title)
{
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setVisible(bool visible)
{
    ActivityStates* states = getActivityStates(NULL);
    sf::Lock lock(states->mutex);

    jint lResult;
    jint lFlags = 0;

    JavaVM* lJavaVM = states->activity->vm;
    JNIEnv* lJNIEnv = states->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {
        return;
    }

    // Retrieves NativeActivity.
    jobject lNativeActivity = states->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE =
        lJNIEnv->GetStaticFieldID(ClassContext,
            "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE =
        lJNIEnv->GetStaticObjectField(ClassContext,
            FieldINPUT_METHOD_SERVICE);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = lJNIEnv->FindClass(
        "android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(
        ClassNativeActivity, "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(
        lNativeActivity, MethodGetSystemService,
        INPUT_METHOD_SERVICE);

    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(
        ClassNativeActivity, "getWindow",
        "()Landroid/view/Window;");
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,
        MethodGetWindow);
    jclass ClassWindow = lJNIEnv->FindClass(
        "android/view/Window");
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(
        ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,
        MethodGetDecorView);

    if (visible)
    {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(
            ClassInputMethodManager, "showSoftInput",
            "(Landroid/view/View;I)Z");
        jboolean lResult = lJNIEnv->CallBooleanMethod(
            lInputMethodManager, MethodShowSoftInput,
            lDecorView, lFlags);
    }
    else
    {
        // Runs lWindow.getViewToken()
        jclass ClassView = lJNIEnv->FindClass(
            "android/view/View");
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(
            ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,
            MethodGetWindowToken);

        // lInputMethodManager.hideSoftInput(...).
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
            ClassInputMethodManager, "hideSoftInputFromWindow",
            "(Landroid/os/IBinder;I)Z");
        jboolean lRes = lJNIEnv->CallBooleanMethod(
            lInputMethodManager, MethodHideSoftInput,
            lBinder, lFlags);
    }

    // Finished with the JVM.
    lJavaVM->DetachCurrentThread();
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setMouseCursorVisible(bool visible)
{
}


////////////////////////////////////////////////////////////////////////////////
void WindowImplAndroid::setKeyRepeatEnabled(bool enabled)
{
}
} // namespace priv
} // namespace sf
