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

#ifndef SFML_WIDGETHANDLE_HPP
#define SFML_WIDGETHANDLE_HPP

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/config.hpp>
#include <sfml/widgets.hpp>


namespace sf
{
////////////////////////////////////////////////////////////////////////////////
// Define a low-level widget handle type, specific to each platform
////////////////////////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_WINDOWS)

    #error This operating system is not supported by SFML library

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    // Widget handle is GtkWidget on Unix - GTK
    typedef GtkWidget* WidgetHandle;

#elif defined(SFML_SYSTEM_MACOS)

    #error This operating system is not supported by SFML library

#endif

} // namespace sf

#endif // SFML_WIDGETHANDLE_HPP
