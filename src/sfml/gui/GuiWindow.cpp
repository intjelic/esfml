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
#include <sfml/gui/GuiWindow.hpp>


namespace sf
{
////////////////////////////////////////////////////////////////////////////////
GuiWindow::GuiWindow() :
m_window(NULL)
{
    gtk_init (NULL, NULL);
    m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(m_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}


////////////////////////////////////////////////////////////////////////////////
GuiWindow::~GuiWindow()
{
}


////////////////////////////////////////////////////////////////////////////////
void GuiWindow::main()
{
    gtk_widget_show(m_window);
    gtk_main();
}


////////////////////////////////////////////////////////////////////////////////
void GuiWindow::onSizeChanged(const Vector2u& newSize, const Vector2u& oldSize)
{
    gtk_window_resize((GtkWindow*)m_window, newSize.x, newSize.y);
}

} // namespace sf
