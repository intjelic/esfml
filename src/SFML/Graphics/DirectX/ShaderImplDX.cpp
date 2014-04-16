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
#include <SFML/Graphics/DirectX/ShaderImplDX.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ShaderImplDX::ShaderImplDX() :
m_device(NULL)
{
    // To implement
}


////////////////////////////////////////////////////////////
ShaderImplDX::~ShaderImplDX()
{
    // To implement
}


////////////////////////////////////////////////////////////
bool ShaderImplDX::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    // To implement

    /*
        HRESULT result;
        LPD3DXBUFFER buffer = NULL;

        if (vertexShaderCode)
        {
            result = D3DXCompileShader(vertexShaderCode,
                                        strlen(vertexShaderCode),
                                        NULL,
                                        NULL,
                                        "main",
                                        "vs_1_1",
                                        0,
                                        &buffer,
                                        NULL,
                                        &m_constantTable);

            if(FAILED(result))
                err() < "Failed to compile vertex shader" << std::endl;

            getContextHandle()->CreateVertexShader((DWORD*)buffer->GetBufferPointer(), &m_vertexShader);

            // check for errors

            buffer->Release();
        }
    */

    return false;
}

////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, float x)
{
    // To implement

    /*
        m_constantTable->SetFloat(m_device, name.c_str(), x);
        // check errors ?
    */
}


////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, float x, float y)
{
    // To implement
}


////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, float x, float y, float z)
{
    // To implement
}


////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, float x, float y, float z, float w)
{
    // To implement
}


////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, const sf::Transform& transform)
{
    // To implement
}


////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, const Texture& texture)
{
    // To implement
}


////////////////////////////////////////////////////////////
void ShaderImplDX::setParameter(const std::string& name, Shader::CurrentTextureType)
{
    // To implement
}

} // namespace priv

} // namespace sf
