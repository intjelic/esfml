////////////////////////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <sfml/graphics/RenderTextureImplFBO.hpp>
#include <sfml/graphics/Texture.hpp>
#include <sfml/graphics/GLCheck.hpp>
#include <sfml/system/error.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////////////////////////
RenderTextureImplFBO::RenderTextureImplFBO() :
m_frameBuffer(0),
m_depthBuffer(0)
{

}


////////////////////////////////////////////////////////////////////////////////
RenderTextureImplFBO::~RenderTextureImplFBO()
{
    #ifndef SFML_EMBEDDED_SYSTEM

    ensureGlContext();

    // Destroy the depth buffer
    if (m_depthBuffer)
    {
        GLuint depthBuffer = static_cast<GLuint>(m_depthBuffer);
        glCheck(glDeleteRenderbuffersEXT(1, &depthBuffer));
    }

    // Destroy the frame buffer
    if (m_frameBuffer)
    {
        GLuint frameBuffer = static_cast<GLuint>(m_frameBuffer);
        glCheck(glDeleteFramebuffersEXT(1, &frameBuffer));
    }

    // Delete the context
    delete m_context;

    #endif
}


////////////////////////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::isAvailable()
{
    #ifndef SFML_EMBEDDED_SYSTEM

        ensureGlContext();

        // Make sure that GLEW is initialized
        priv::ensureGlewInit();

        return GLEW_EXT_framebuffer_object != 0;

    #else

        return false;

    #endif
}


////////////////////////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::create(unsigned int width, unsigned int height, unsigned int textureId, bool depthBuffer)
{
    #ifndef SFML_EMBEDDED_SYSTEM

    // Create the context
    m_context = new Context;

    // Create the framebuffer object
    GLuint frameBuffer = 0;
    glCheck(glGenFramebuffersEXT(1, &frameBuffer));
    m_frameBuffer = static_cast<unsigned int>(frameBuffer);
    if (!m_frameBuffer)
    {
        err() << "Impossible to create render texture (failed to create the frame buffer object)" << std::endl;
        return false;
    }
    glCheck(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer));

    // Create the depth buffer if requested
    if (depthBuffer)
    {
        GLuint depth = 0;
        glCheck(glGenRenderbuffersEXT(1, &depth));
        m_depthBuffer = static_cast<unsigned int>(depth);
        if (!m_depthBuffer)
        {
            err() << "Impossible to create render texture (failed to create the attached depth buffer)" << std::endl;
            return false;
        }
        glCheck(glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer));
        glCheck(glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height));
        glCheck(glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthBuffer));
    }

    // Link the texture to the frame buffer
    glCheck(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0));

    // A final check, just to be sure...
    GLenum status = glCheck(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        glCheck(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));
        err() << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
        return false;
    }

    return true;

    #else

    return false;

    #endif
}


////////////////////////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::activate(bool active)
{
    #ifndef SFML_EMBEDDED_SYSTEM
    return m_context->setActive(active);
    #endif
}


////////////////////////////////////////////////////////////////////////////////
void RenderTextureImplFBO::updateTexture(unsigned int)
{
    glCheck(glFlush());
}

} // namespace priv

} // namespace sf