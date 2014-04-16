////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/OpenGL/ShaderImplGL.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Err.hpp>


#ifndef SFML_OPENGL_ES

namespace
{
    // Retrieve the maximum number of texture units available
    GLint getMaxTextureUnits()
    {
        GLint maxUnits = 0;
        glCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
        return maxUnits;
    }
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ShaderImplGL::ShaderImplGL() :
m_shaderProgram (0),
m_currentTexture(-1),
m_textures      (),
m_params        ()
{
}


////////////////////////////////////////////////////////////
ShaderImplGL::~ShaderImplGL()
{
    ensureContext();

    // Destroy effect program
    if (m_shaderProgram)
        glCheck(glDeleteObjectARB(m_shaderProgram));
}


////////////////////////////////////////////////////////////
bool ShaderImplGL::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    ensureContext();

    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_params.clear();
    
    // Create the program
    m_shaderProgram = glCheck(glCreateProgramObjectARB());

    // Create the vertex shader if needed
    if (vertexShaderCode)
    {
        // Create and compile the shader
        GLhandleARB vertexShader = glCheck(glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB));
        glCheck(glShaderSourceARB(vertexShader, 1, &vertexShaderCode, NULL));
        glCheck(glCompileShaderARB(vertexShader));

        // Check the compile log
        GLint success;
        glCheck(glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(glGetInfoLogARB(vertexShader, sizeof(log), 0, log));
            err() << "Failed to compile vertex shader:" << std::endl
                  << log << std::endl;
            glCheck(glDeleteObjectARB(vertexShader));
            glCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(glAttachObjectARB(m_shaderProgram, vertexShader));
        glCheck(glDeleteObjectARB(vertexShader));
    }

    // Create the fragment shader if needed
    if (fragmentShaderCode)
    {
        // Create and compile the shader
        GLhandleARB fragmentShader = glCheck(glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB));
        glCheck(glShaderSourceARB(fragmentShader, 1, &fragmentShaderCode, NULL));
        glCheck(glCompileShaderARB(fragmentShader));

        // Check the compile log
        GLint success;
        glCheck(glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(glGetInfoLogARB(fragmentShader, sizeof(log), 0, log));
            err() << "Failed to compile fragment shader:" << std::endl
                  << log << std::endl;
            glCheck(glDeleteObjectARB(fragmentShader));
            glCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(glAttachObjectARB(m_shaderProgram, fragmentShader));
        glCheck(glDeleteObjectARB(fragmentShader));
    }

    // Link the program
    glCheck(glLinkProgramARB(m_shaderProgram));

    // Check the link log
    GLint success;
    glCheck(glGetObjectParameterivARB(m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        glCheck(glGetInfoLogARB(m_shaderProgram, sizeof(log), 0, log));
        err() << "Failed to link shader:" << std::endl
              << log << std::endl;
        glCheck(glDeleteObjectARB(m_shaderProgram));
        m_shaderProgram = 0;
        return false;
    }

    // Force an OpenGL flush, so that the shader will appear updated
    // in all contexts immediately (solves problems in multi-threaded apps)
    glCheck(glFlush());

    return true;
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x)
{
    ensureContext();

    // Enable program
    GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
    glCheck(glUseProgramObjectARB(m_shaderProgram));

    // Get parameter location and assign it new values
    GLint location = getParamLocation(name);
    if (location != -1)
    {
        glCheck(glUniform1fARB(location, x));
    }

    // Disable program
    glCheck(glUseProgramObjectARB(program));
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x, float y)
{
    ensureContext();

    // Enable program
    GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
    glCheck(glUseProgramObjectARB(m_shaderProgram));

    // Get parameter location and assign it new values
    GLint location = getParamLocation(name);
    if (location != -1)
    {
        glCheck(glUniform2fARB(location, x, y));
    }

    // Disable program
    glCheck(glUseProgramObjectARB(program));
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x, float y, float z)
{
    ensureContext();

    // Enable program
    GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
    glCheck(glUseProgramObjectARB(m_shaderProgram));

    // Get parameter location and assign it new values
    GLint location = getParamLocation(name);
    if (location != -1)
    {
        glCheck(glUniform3fARB(location, x, y, z));
    }

    // Disable program
    glCheck(glUseProgramObjectARB(program));
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x, float y, float z, float w)
{
    ensureContext();

    // Enable program
    GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
    glCheck(glUseProgramObjectARB(m_shaderProgram));

    // Get parameter location and assign it new values
    GLint location = getParamLocation(name);
    if (location != -1)
    {
        glCheck(glUniform4fARB(location, x, y, z, w));
    }

    // Disable program
    glCheck(glUseProgramObjectARB(program));
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, const sf::Transform& transform)
{
    ensureContext();

    // Enable program
    GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
    glCheck(glUseProgramObjectARB(m_shaderProgram));

    // Get parameter location and assign it new values
    GLint location = getParamLocation(name);
    if (location != -1)
    {
        glCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.getMatrix()));
    }

    // Disable program
    glCheck(glUseProgramObjectARB(program));
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, const Texture& texture)
{
    ensureContext();

    // Find the location of the variable in the shader
    int location = getParamLocation(name);
    if (location != -1)
    {
        // Store the location -> texture mapping
        TextureTable::iterator it = m_textures.find(location);
        if (it == m_textures.end())
        {
            // New entry, make sure there are enough texture units
            static const GLint maxUnits = getMaxTextureUnits();
            if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
            {
                err() << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
                return;
            }

            m_textures[location] = &texture;
        }
        else
        {
            // Location already used, just replace the texture
            it->second = &texture;
        }
    }
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, Shader::CurrentTextureType)
{
    ensureContext();

    // Find the location of the variable in the shader
    m_currentTexture = getParamLocation(name);
}


////////////////////////////////////////////////////////////
void ShaderImplGL::bindTextures() const
{
    TextureTable::const_iterator it = m_textures.begin();
    for (std::size_t i = 0; i < m_textures.size(); ++i)
    {
        GLint index = static_cast<GLsizei>(i + 1);
        glCheck(glUniform1iARB(it->first, index));
        glCheck(glActiveTextureARB(GL_TEXTURE0_ARB + index));
        Texture::bind(it->second);
        ++it;
    }

    // Make sure that the texture unit which is left active is the number 0
    glCheck(glActiveTextureARB(GL_TEXTURE0_ARB));
}


////////////////////////////////////////////////////////////
int ShaderImplGL::getParamLocation(const std::string& name)
{
    // Check the cache
    ParamTable::const_iterator it = m_params.find(name);
    if (it != m_params.end())
    {
        // Already in cache, return it
        return it->second;
    }
    else
    {
        // Not in cache, request the location from OpenGL
        int location = glCheck(glGetUniformLocationARB(m_shaderProgram, name.c_str()));
        if (location != -1)
        {
            // Location found: add it to the cache
            m_params.insert(std::make_pair(name, location));
        }
        else
        {
            // Error: location not found
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;
        }

        return location;
    }
}

} // namespace priv

} // namespace sf


#else // SFML_OPENGL_ES

// OpenGL ES 1 does't support GLSL shaders at all, we have to provide an empty implementation

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ShaderImplGL::ShaderImplGL() :
m_shaderProgram (0),
m_currentTexture(-1)
{
}


////////////////////////////////////////////////////////////
ShaderImplGL::~ShaderImplGL()
{
}


////////////////////////////////////////////////////////////
bool ShaderImplGL::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    return false;
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x)
{
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x, float y)
{
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x, float y, float z)
{
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, float x, float y, float z, float w)
{
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, const sf::Transform& transform)
{
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, const Texture& texture)
{
}


////////////////////////////////////////////////////////////
void ShaderImplGL::setParameter(const std::string& name, Shader::CurrentTextureType)
{
}

} // namespace priv

} // namespace sf

#endif // SFML_OPENGL_ES
