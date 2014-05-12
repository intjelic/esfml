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
#include <SFML/Graphics/OpenGL/RenderTargetImplGL.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderTargetImplGL::RenderTargetImplGL()
{
    // To implement
}


////////////////////////////////////////////////////////////
RenderTargetImplGL::~RenderTargetImplGL()
{
    // To implement
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::clear(const Color& color)
{
    glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type)
{
    // GL_QUADS is unavailable on OpenGL ES
    #ifdef SFML_OPENGL_ES
        if (type == Quads)
        {
            err() << "sf::Quads primitive type is not supported on OpenGL ES platforms, drawing skipped" << std::endl;
            return;
        }
        #define GL_QUADS 0
    #endif

    // Setup the pointers to the vertices' components
    if (vertices)
    {
        const char* data = reinterpret_cast<const char*>(vertices);
        glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + 0));
        glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8));
        glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12));
    }

    // Find the OpenGL primitive type
    static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                   GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[type];

    // Draw the primitives
    glCheck(glDrawArrays(mode, 0, vertexCount));
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::pushStates()
{
    #ifdef SFML_DEBUG
        // make sure that the user didn't leave an unchecked OpenGL error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            err() << "OpenGL error (" << error << ") detected in user code, "
                  << "you should check for errors with glGetError()"
                  << std::endl;
        }
    #endif

    #ifndef SFML_OPENGL_ES
        glCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
        glCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));
    #endif
    glCheck(glMatrixMode(GL_MODELVIEW));
    glCheck(glPushMatrix());
    glCheck(glMatrixMode(GL_PROJECTION));
    glCheck(glPushMatrix());
    glCheck(glMatrixMode(GL_TEXTURE));
    glCheck(glPushMatrix());
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::popStates()
{
    glCheck(glMatrixMode(GL_PROJECTION));
    glCheck(glPopMatrix());
    glCheck(glMatrixMode(GL_MODELVIEW));
    glCheck(glPopMatrix());
    glCheck(glMatrixMode(GL_TEXTURE));
    glCheck(glPopMatrix());
    #ifndef SFML_OPENGL_ES
        glCheck(glPopClientAttrib());
        glCheck(glPopAttrib());
    #endif
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::resetStates()
{
    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    // Define the default OpenGL states
    glCheck(glDisable(GL_CULL_FACE));
    glCheck(glDisable(GL_LIGHTING));
    glCheck(glDisable(GL_DEPTH_TEST));
    glCheck(glDisable(GL_ALPHA_TEST));
    glCheck(glEnable(GL_TEXTURE_2D));
    glCheck(glEnable(GL_BLEND));
    glCheck(glMatrixMode(GL_MODELVIEW));
    glCheck(glEnableClientState(GL_VERTEX_ARRAY));
    glCheck(glEnableClientState(GL_COLOR_ARRAY));
    glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::applyCurrentView(const View& view, IntRect& viewport, Vector2u& size)
{
    // Set the viewport
    int top = size.y - (viewport.top + viewport.height);
    glCheck(glViewport(viewport.left, top, viewport.width, viewport.height));

    // Set the projection matrix
    glCheck(glMatrixMode(GL_PROJECTION));
    glCheck(glLoadMatrixf(view.getTransform().getMatrix()));

    // Go back to model-view mode
    glCheck(glMatrixMode(GL_MODELVIEW));
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::applyBlendMode(BlendMode mode)
{
    switch (mode)
    {
        // glBlendFuncSeparate is used when available to avoid an incorrect alpha value when the target
        // is a RenderTexture -- in this case the alpha value must be written directly to the target buffer

        // Alpha blending
        default :
        case BlendAlpha :
            if (GLEXT_blend_func_separate)
            {
                glCheck(GLEXT_glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
            }
            else
            {
                glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            }

            break;

        // Additive blending
        case BlendAdd :
            if (GLEXT_blend_func_separate)
            {
                glCheck(GLEXT_glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE));
            }
            else
            {
                glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
            }

            break;

        // Multiplicative blending
        case BlendMultiply :
            glCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
            break;

        // No blending
        case BlendNone :
            glCheck(glBlendFunc(GL_ONE, GL_ZERO));
            break;
    }
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::applyTransform(const Transform& transform)
{
    // No need to call glMatrixMode(GL_MODELVIEW), it is always the
    // current mode (for optimization purpose, since it's the most used)
    glCheck(glLoadMatrixf(transform.getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTargetImplGL::applyTexture(const Texture* texture)
{
    Texture::bind(texture, Texture::Pixels);
}

} // namespace priv

} // namespace sf
