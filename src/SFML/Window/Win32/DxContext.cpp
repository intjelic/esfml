////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013-2014 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/WindowImpl.hpp> // included first to avoid a warning about macro redefinition
#include <SFML/Window/Win32/DxContext.hpp>
#include <SFML/Window/Win32/Interface.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
DxContext::DxContext(DxContext* shared) :
m_window       (NULL),
m_deviceContext(NULL),
m_interface    (NULL),
m_context      (NULL),
m_ownsWindow   (true)
{
<<<<<<< Updated upstream
	// Grab the unique DirectX interface instance
	m_interface = OpenInterface();
	
=======
    // Grab the unique DirectX interface instance
    m_interface = OpenInterface();

>>>>>>> Stashed changes
    // Create a dummy window (disabled and hidden)
    m_window = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(m_window, SW_HIDE);
    m_deviceContext = GetDC(m_window);

    // Create the context
    if (m_deviceContext)
        createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
}


////////////////////////////////////////////////////////////
DxContext::DxContext(DxContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel) :
m_window       (NULL),
m_deviceContext(NULL),
m_interface    (NULL),
m_context      (NULL),
m_ownsWindow   (false)
{
<<<<<<< Updated upstream
	// Grab the unique DirectX interface instance
	m_interface = OpenInterface();
	
=======
    // Grab the unique DirectX interface instance
    m_interface = OpenInterface();

>>>>>>> Stashed changes
    // Get the owner window and its device context
    m_window = owner->getSystemHandle();
    m_deviceContext = GetDC(m_window);

    // Create the context
    if (m_deviceContext)
        createContext(shared, bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////
DxContext::DxContext(DxContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height) :
m_window       (NULL),
m_deviceContext(NULL),
m_interface    (NULL),
m_context      (NULL),
m_ownsWindow   (true)
{
<<<<<<< Updated upstream
	// Grab the unique DirectX interface instance
	m_interface = OpenInterface();
	
=======
    // Grab the unique DirectX interface instance
    m_interface = OpenInterface();

>>>>>>> Stashed changes
    // Create the hidden window
    m_window = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(m_window, SW_HIDE);
    m_deviceContext = GetDC(m_window);

    // Create the context
    if (m_deviceContext)
        createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////
DxContext::~DxContext()
{
<<<<<<< Updated upstream
	// Release the DirectX context
	m_context->Release();
	
	// Drop the DirectX interface instance
	CloseInterface(m_interface);
=======
    // Release the DirectX context
    m_context->Release();

    // Drop the DirectX interface instance
    CloseInterface(m_interface);
>>>>>>> Stashed changes
}


////////////////////////////////////////////////////////////
bool DxContext::makeCurrent()
{
    // DirectX contexts aren't states thus making them current doesn't make sense
    return true;
}


////////////////////////////////////////////////////////////
void DxContext::display()
{
    if (m_deviceContext && m_context)
    {
        m_context->EndScene();
        m_context->Present(NULL, NULL, NULL, NULL);
        m_context->BeginScene();
    }
}


////////////////////////////////////////////////////////////
void DxContext::setVerticalSyncEnabled(bool enabled)
{
    // To be implemented
}


////////////////////////////////////////////////////////////
void DxContext::createContext(DxContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
<<<<<<< Updated upstream
	D3DPRESENT_PARAMETERS params;
	
	ZeroMemory(&params, sizeof(params));
	params.Windowed = TRUE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = m_window;
	
	m_interface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&params,
		&m_context);
=======
    D3DPRESENT_PARAMETERS params;

    ZeroMemory(&params, sizeof(params));
    params.Windowed = TRUE;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = m_window;

    m_interface->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        m_window,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &params,
        &m_context);
>>>>>>> Stashed changes
}

} // namespace priv

} // namespace sf
