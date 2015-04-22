////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Blackberry/WindowImplBlackberry.hpp>
#include <SFML/System/Err.hpp>
#include <bps/event.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/sensor.h>
#include <sys/keycodes.h>
#include <input/screen_helpers.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplBlackberry::WindowImplBlackberry(WindowHandle handle) :
m_window  (NULL),
m_context (NULL),
m_buffer  (NULL),
m_display (NULL),
m_usage   (0),
m_format  (0),
m_width   (0),
m_height  (0)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
WindowImplBlackberry::WindowImplBlackberry(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings) :
m_window  (NULL),
m_context (NULL),
m_buffer  (NULL),
m_display (NULL),
m_usage   (0),
m_format  (0),
m_width   (0),
m_height  (0)
{
    // Initialize BPS library
    bps_initialize();

    // Create the context
    int error = screen_create_context(&m_context, SCREEN_APPLICATION_CONTEXT);
    checkError(error, "Failed to create the context");

    // Create the window
    error = screen_create_window(&m_window, m_context);
    checkError(error, "Failed to create the window");

    // Set the screen format
    m_format = SCREEN_FORMAT_RGBX8888;
    error = screen_set_window_property_iv(m_window, SCREEN_PROPERTY_FORMAT, &m_format);
    checkError(error, "Failed to set the screen format");

    // Set the screen usage
    m_usage = SCREEN_USAGE_OPENGL_ES1 | SCREEN_USAGE_ROTATION;
    error = screen_set_window_property_iv(m_window, SCREEN_PROPERTY_USAGE, &m_usage);
    checkError(error, "Failed to set the screen usage");

    // Get the screen display
    error = screen_get_window_property_pv(m_window, SCREEN_PROPERTY_DISPLAY, (void **)&m_display);
    checkError(error, "Failed to get the screen display");

    // Get screen size
    int screen_resolution[2];
    error = screen_get_display_property_iv(m_display, SCREEN_PROPERTY_SIZE, screen_resolution);
    checkError(error, "Failed to get the screen size");

    int angle = atoi(getenv("ORIENTATION"));

    // Get the screen mode
    screen_display_mode_t screen_mode;
    error = screen_get_display_property_pv(m_display, SCREEN_PROPERTY_MODE, (void**)&screen_mode);
    checkError(error, "Failed to get the screen mode");

    // Get the screen buffer size
    int size[2];
    error = screen_get_window_property_iv(m_window, SCREEN_PROPERTY_BUFFER_SIZE, size);
    checkError(error, "Failed to get the screen buffer size");


    int buffer_size[2] = {size[0], size[1]};

    if ((angle == 0) || (angle == 180)) {
        if (((screen_mode.width > screen_mode.height) && (size[0] < size[1])) ||
            ((screen_mode.width < screen_mode.height) && (size[0] > size[1]))) {
                buffer_size[1] = size[0];
                buffer_size[0] = size[1];
        }
    } else if ((angle == 90) || (angle == 270)){
        if (((screen_mode.width > screen_mode.height) && (size[0] > size[1])) ||
            ((screen_mode.width < screen_mode.height && size[0] < size[1]))) {
                buffer_size[1] = size[0];
                buffer_size[0] = size[1];
        }
    } else {
         err() << "Navigator returned an unexpected orientation angle." << std::endl;
    }

    m_width = buffer_size[0];
    m_height = buffer_size[1];

    // Set the screen buffer size
    error = screen_set_window_property_iv(m_window, SCREEN_PROPERTY_BUFFER_SIZE, buffer_size);
    checkError(error, "Failed to set the screen buffer size");

    // Set the screen rotation
    error = screen_set_window_property_iv(m_window, SCREEN_PROPERTY_ROTATION, &angle);
    checkError(error, "Failed to set the screen rotation");

    // Create window buffers
    error = screen_create_window_buffers(m_window, 2);
    checkError(error, "Failed to create window buffer");

    // Start requesting events
    navigator_request_events(0);
    screen_request_events(m_context);
}


////////////////////////////////////////////////////////////
WindowImplBlackberry::~WindowImplBlackberry()
{
    // Destroy the window
    int error = screen_destroy_window(m_window);
    checkError(error, "Failed to destroy the window");

    // Destroy the screen context
    screen_destroy_context(m_context);
    checkError(error, "Failed to destroy the screen context");

    // Shut down BPS library
    bps_shutdown();
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplBlackberry::getSystemHandle() const
{
    return (void*)m_window;
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::processEvents()
{
    bps_event_t *event = NULL;

    while(bps_get_event(&event, 0) == BPS_SUCCESS && event != NULL)
    {
        int domain = bps_event_get_domain(event);

        if(domain == navigator_get_domain())
        {
            handleNavigatorEvent(event);
        }
        else if(domain == screen_get_domain())
        {
            handleScreenEvent(event);
        }
        else if(domain == sensor_get_domain())
        {
            handleSensorEvent(event);
        }
    }
}


////////////////////////////////////////////////////////////
Vector2i WindowImplBlackberry::getPosition() const
{
    // Not applicable
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setPosition(const Vector2i& position)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
Vector2u WindowImplBlackberry::getSize() const
{
    return Vector2u(m_width, m_height);
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setSize(const Vector2u& size)
{
    if(size.x > size.y)
        navigator_set_orientation(0, NULL);
    else
        navigator_set_orientation(90, NULL);
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setTitle(const String& title)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setMouseCursorVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::setKeyRepeatEnabled(bool enabled)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::requestFocus()
{
    // To be implemented
}


////////////////////////////////////////////////////////////
bool WindowImplBlackberry::hasFocus() const
{
    // To be implemented
    return true;
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::checkError(int error, const char* message)
{
    if(error)
        err() << message << std::endl;
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::handleNavigatorEvent(bps_event_t *_event)
{
    int code = bps_event_get_code(_event);
    Event event;

    switch(code)
    {
        case NAVIGATOR_EXIT:
            event.type = sf::Event::Closed;
            pushEvent(event);
            break;

        case NAVIGATOR_WINDOW_ACTIVE:
            event.type = sf::Event::GainedFocus;
            pushEvent(event);
            break;

        case NAVIGATOR_WINDOW_INACTIVE:
            event.type = sf::Event::LostFocus;
            pushEvent(event);
            break;

        case NAVIGATOR_ORIENTATION_CHECK:
            navigator_orientation_check_response(_event, true);
            event.type = sf::Event::Resized;
            event.size.width = 0;
            event.size.height = 0;
            pushEvent(event);
            break;

        case NAVIGATOR_ORIENTATION:
            navigator_done_orientation(_event);
            break;
    }
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::handleScreenEvent(bps_event_t* _event)
{
    screen_event_t event = screen_event_get_event(_event);

    int type;
    screen_get_event_property_iv(event, SCREEN_PROPERTY_TYPE, &type);

    switch (type)
    {
        case SCREEN_EVENT_KEYBOARD:
            handleKeyboardEvent(event);
            break;

        case SCREEN_EVENT_MTOUCH_TOUCH:
        case SCREEN_EVENT_MTOUCH_RELEASE:
        case SCREEN_EVENT_MTOUCH_MOVE:
            handleTouchEvent(type, event);
            break;

        case SCREEN_EVENT_POINTER:
            handleMouseEvent(event);
            break;
    }
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::handleSensorEvent(bps_event_t *event)
{
    // To be implemented
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::handleKeyboardEvent(screen_event_t& _event)
{
    // Retrieve event raw data
    int flags, value;
    screen_get_event_property_iv(_event, SCREEN_PROPERTY_KEY_FLAGS, &flags);
    screen_get_event_property_iv(_event, SCREEN_PROPERTY_KEY_SYM, &value);

    // Decode data to create our own key event
    // @todo: fill alt, control, shift and system attribute
    Event event;
    event.type = (flags & KEY_DOWN) ? Event::KeyPressed : Event::KeyReleased;
    event.key.code = bbKeyToSF(value);
    event.key.alt = false;     // type & KEYMODBIT_ALT;
    event.key.control = false; // type & KEYMODBIT_CTRL;
    event.key.shift = false;   // type & KEYMODBIT_SHIFT;
    event.key.system = false;  // Is there an equivalent ?

    pushEvent(event);

    // Check if it has triggered a printable character
    if (event.type == Event::KeyPressed && (flags & KEY_SYM_VALID))
    {
        // Some non-printable characters have corresponding unicode values
        if (value >= KEYCODE_PC_KEYS && value <= UNICODE_PRIVATE_USE_AREA_LAST)
        {
            switch(value)
            {
                case KEYCODE_BACKSPACE:
                    value = 0x0008;
                case KEYCODE_TAB:
                    value = 0x0009;
                case KEYCODE_KP_ENTER:
                case KEYCODE_RETURN:
                    value = 0x000A;
                case KEYCODE_ESCAPE:
                    value = 0x001B;
                default:
                    value = 0;
            }
        }

        // @todo: does that match the SFML policy ? what about unicode character being repeated ?
        if (value)
        {
            event.type = Event::TextEntered;
            event.text.unicode = static_cast<Uint32>(value);
            pushEvent(event);
        }
    }
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::handleTouchEvent(int type, screen_event_t& _event)
{
    mtouch_event_t data;
    screen_get_mtouch_event(_event, &data, 0);

    Event event;
    event.type = getTouchType(type);
    event.touch.finger = data.contact_id;
    event.touch.x = data.x;
    event.touch.y = data.y;
    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplBlackberry::handleMouseEvent(screen_event_t& _event)
{
    int buttons, position[2], wheel;
    screen_get_event_property_iv(_event, SCREEN_PROPERTY_BUTTONS, &buttons);
    screen_get_event_property_iv(_event, SCREEN_PROPERTY_SOURCE_POSITION, position);
    screen_get_event_property_iv(_event, SCREEN_PROPERTY_MOUSE_WHEEL, &wheel);

    static bool isButtonPressed = 0;

    Event event;

    if(wheel)
    {
        event.type = Event::MouseWheelMoved;
        event.mouseWheel.delta = wheel;
        event.mouseWheel.x = position[0];
        event.mouseWheel.y = position[1];
        pushEvent(event);
    }

    if((buttons & SCREEN_LEFT_MOUSE_BUTTON) != (isButtonPressed & SCREEN_LEFT_MOUSE_BUTTON))
    {
        event.type = (buttons & SCREEN_LEFT_MOUSE_BUTTON) ? Event::MouseButtonPressed : Event::MouseButtonReleased;
        event.mouseButton.button = Mouse::Left;
        event.mouseButton.x = position[0];
        event.mouseButton.y = position[1];
        pushEvent(event);

        isButtonPressed |= SCREEN_LEFT_MOUSE_BUTTON;
        return;
    }

    if((buttons & SCREEN_RIGHT_MOUSE_BUTTON) != (isButtonPressed & SCREEN_RIGHT_MOUSE_BUTTON))
    {
        event.type = (buttons & SCREEN_RIGHT_MOUSE_BUTTON) ? Event::MouseButtonPressed : Event::MouseButtonReleased;
        event.mouseButton.button = Mouse::Right;
        event.mouseButton.x = position[0];
        event.mouseButton.y = position[1];
        pushEvent(event);

        isButtonPressed |= SCREEN_RIGHT_MOUSE_BUTTON;
        return;
    }

    if((buttons & SCREEN_MIDDLE_MOUSE_BUTTON) != (isButtonPressed & SCREEN_MIDDLE_MOUSE_BUTTON))
    {
        event.type = (buttons & SCREEN_MIDDLE_MOUSE_BUTTON) ? Event::MouseButtonPressed : Event::MouseButtonReleased;
        event.mouseButton.button = Mouse::Middle;
        event.mouseButton.x = position[0];
        event.mouseButton.y = position[1];
        pushEvent(event);

        isButtonPressed |= SCREEN_MIDDLE_MOUSE_BUTTON;
        return;
    }

    event.type = Event::MouseMoved;
    event.mouseMove.x = position[0];
    event.mouseMove.y = position[1];
    pushEvent(event);

}


////////////////////////////////////////////////////////////
Keyboard::Key WindowImplBlackberry::bbKeyToSF(int key)
{
    // @todo: what about "case 0x20AC: return Keyboard::KEY_EURO; "

    switch (key)
    {
        case KEYCODE_A                  : return Keyboard::A;
        case KEYCODE_B                  : return Keyboard::B;
        case KEYCODE_C                  : return Keyboard::C;
        case KEYCODE_D                  : return Keyboard::D;
        case KEYCODE_E                  : return Keyboard::E;
        case KEYCODE_F                  : return Keyboard::F;
        case KEYCODE_G                  : return Keyboard::G;
        case KEYCODE_H                  : return Keyboard::H;
        case KEYCODE_I                  : return Keyboard::I;
        case KEYCODE_J                  : return Keyboard::J;
        case KEYCODE_K                  : return Keyboard::K;
        case KEYCODE_L                  : return Keyboard::L;
        case KEYCODE_M                  : return Keyboard::M;
        case KEYCODE_N                  : return Keyboard::N;
        case KEYCODE_O                  : return Keyboard::O;
        case KEYCODE_P                  : return Keyboard::P;
        case KEYCODE_Q                  : return Keyboard::Q;
        case KEYCODE_R                  : return Keyboard::R;
        case KEYCODE_S                  : return Keyboard::S;
        case KEYCODE_T                  : return Keyboard::T;
        case KEYCODE_U                  : return Keyboard::U;
        case KEYCODE_V                  : return Keyboard::V;
        case KEYCODE_W                  : return Keyboard::W;
        case KEYCODE_X                  : return Keyboard::X;
        case KEYCODE_Y                  : return Keyboard::Y;
        case KEYCODE_Z                  : return Keyboard::Z;
        default : return Keyboard::Unknown;
    }
}


////////////////////////////////////////////////////////////
Event::EventType WindowImplBlackberry::getTouchType(int type)
{
    switch(type)
    {
        case SCREEN_EVENT_MTOUCH_TOUCH:   return Event::TouchBegan;
        case SCREEN_EVENT_MTOUCH_RELEASE: return Event::TouchEnded;
        case SCREEN_EVENT_MTOUCH_MOVE:    return Event::TouchMoved;
    }
}

} // namespace priv

} // namespace sf
