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

#ifndef SFML_BUTTON_HPP
#define SFML_BUTTON_HPP

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <sfml/gui/export.hpp>
#include <sfml/gui/Widget.hpp>


namespace sf
{
class SFML_GUI_API Button : public Widget
{
public :

    Button();
    ~Button();

     GtkWidget* getWidgetHandler();

protected :

    virtual void onPaint(RenderTarget& target, const RenderStates& states);
    virtual void onSizeChanged(const Vector2u& newSize, const Vector2u& oldSize);

private :

    static gboolean onDraw(GtkWidget *widget, cairo_t *cr, gpointer user_data);

    GtkWidget* m_handler;
    RenderTexture m_paintingArea; ///< The off-screen to handle extra drawing
    gulong m_paintingHandler;     ///< The handler id of our callback draw function
    cairo_t* m_cairoContext;      ///< Temporary buffer to save the cairo context

};

} // namespace sf


#endif // SFML_BUTTON_HPP
