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
#include <sfml/gui/Button.hpp>


namespace sf
{
////////////////////////////////////////////////////////////////////////////////
Button::Button() :
Widget            (),
m_handler         (NULL),
m_paintingArea    (),
m_paintingHandler (0),
m_cairoContext    (NULL)
{
    m_handler = gtk_button_new();

    // Warn the SFML widget tree when it's being drawn, we'll need the handler
    // id later.
    m_paintingHandler = g_signal_connect(m_handler, "draw", G_CALLBACK(&onDraw), (gpointer)this);
}


////////////////////////////////////////////////////////////////////////////////
Button::~Button()
{
}


////////////////////////////////////////////////////////////////////////////////
GtkWidget* Button::getWidgetHandler()
{
    return m_handler;
}


////////////////////////////////////////////////////////////////////////////////
void Button::onSizeChanged(const Vector2u& newSize, const Vector2u& oldSize)
{
    // Resize the offscreen target
    if (newSize.x > 0 && newSize.y > 0)
    {
        m_paintingArea.create(newSize.x, newSize.y);
    }

    gtk_widget_set_size_request(m_handler, newSize.x, newSize.y);
}


////////////////////////////////////////////////////////////////////////////////
void Button::onPaint(RenderTarget& target, const RenderStates& states)
{
    // Draw the default appearance
    g_signal_handler_disconnect(m_handler, m_paintingHandler);
    gtk_widget_draw(m_handler, m_cairoContext);
    m_paintingHandler = g_signal_connect(m_handler, "draw", G_CALLBACK(&onDraw), (gpointer)this);
}


////////////////////////////////////////////////////////////////////////////////
gboolean Button::onDraw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    // The widget must render itself, we will forward the event to the SFML
    // widget

    Button* m = (Button*)user_data;

    // Prepare the paintaing area in case of extra drawing
    m->m_paintingArea.clear(sf::Color::Transparent);
    m->m_cairoContext = cr; // Save the cairo context to a temporary buffer

    // Let the SFML widget draw itself
    m->onPaint(m->m_paintingArea, RenderStates());

    // Add the extra drawings
    m->m_paintingArea.display();
    const Texture& texture = m->m_paintingArea.getTexture();
    Image image = texture.copyToImage();

    int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, image.getSize().x);
    cairo_surface_t* surface = cairo_image_surface_create_for_data((Uint8*)image.getPixelsPtr(), CAIRO_FORMAT_ARGB32, image.getSize().x, image.getSize().y, stride);
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    cairo_surface_destroy(surface);

    return TRUE;
}

} // namespace sf
