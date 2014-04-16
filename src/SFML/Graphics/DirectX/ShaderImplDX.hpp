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

#ifndef SFML_SHADERIMPLDX_HPP
#define SFML_SHADERIMPLDX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/ShaderImpl.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief DirectX implementation of ShaderImpl
///
////////////////////////////////////////////////////////////
class ShaderImplDX : public ShaderImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ShaderImplDX();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ShaderImplDX();

    ////////////////////////////////////////////////////////////
    /// \brief Compile the shader(s) and create the program
    ///
    /// If one of the arguments is NULL, the corresponding shader
    /// is not created.
    ///
    /// \param vertexShaderCode   Source code of the vertex shader
    /// \param fragmentShaderCode Source code of the fragment shader
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool compile(const char* vertexShaderCode, const char* fragmentShaderCode);

    ////////////////////////////////////////////////////////////
    /// \brief Change a float parameter of the shader
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    Value to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, float x);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 2-components vector parameter of the shader
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    First component of the value to assign
    /// \param y    Second component of the value to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, float x, float y);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 3-components vector parameter of the shader
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    First component of the value to assign
    /// \param y    Second component of the value to assign
    /// \param z    Third component of the value to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 4-components vector parameter of the shader
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    First component of the value to assign
    /// \param y    Second component of the value to assign
    /// \param z    Third component of the value to assign
    /// \param w    Fourth component of the value to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, float x, float y, float z, float w);

    ////////////////////////////////////////////////////////////
    /// \brief Change a matrix parameter of the shader
    ///
    /// \param name      Name of the parameter in the shader
    /// \param transform Transform to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, const sf::Transform& transform);

    ////////////////////////////////////////////////////////////
    /// \brief Change a texture parameter of the shader
    ///
    /// \param name    Name of the texture in the shader
    /// \param texture Texture to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, const Texture& texture);

    ////////////////////////////////////////////////////////////
    /// \brief Change a texture parameter of the shader
    ///
    /// \param name Name of the texture in the shader
    ///
    ////////////////////////////////////////////////////////////
    virtual void setParameter(const std::string& name, Shader::CurrentTextureType);

private:
    IDirect3DDevice9* m_device;
    LPDIRECT3DVERTEXDECLARATION9 m_vertexDecl;
    LPDIRECT3DVERTEXSHADER9      m_vertexShader;
    //LPD3DXCONSTANTTABLE          m_constantTable;
    LPDIRECT3DPIXELSHADER9       m_pixelShader;

};

} // namespace priv

} // namespace sf

#endif // SFML_SHADERIMPLDX_HPP
