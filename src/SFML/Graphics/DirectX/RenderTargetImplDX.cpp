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
#include <SFML/Graphics/DirectX/RenderTargetImplDX.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/DirectX.hpp>
#include <d3dx9math.h>
#include <vector>


namespace
{
    #define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

    struct CUSTOMVERTEX
    {
        FLOAT x, y, z;
        DWORD color;
        FLOAT texX, texY;
    };

    sf::Vector2u textureSize;
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderTargetImplDX::RenderTargetImplDX()
{
    IDirect3DDevice9* context = getContextHandle();

    context->CreateStateBlock(D3DSBT_ALL, &m_states);
}


////////////////////////////////////////////////////////////
RenderTargetImplDX::~RenderTargetImplDX()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::clear(const Color& color)
{
    IDirect3DDevice9* context = getContextHandle();

    context->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), 1.0f, 0);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type)
{
    IDirect3DDevice9* context = getContextHandle();

    // This is a slow operation and a workaround to the inability to
    // declare a FVF matching the current SFML vertex layout.
    std::vector<CUSTOMVERTEX> repackedVertices;
    repackedVertices.reserve(vertexCount);

    for (unsigned int i = 0; i < vertexCount; i++)
    {
        CUSTOMVERTEX vertex = {
            vertices[i].position.x,
            vertices[i].position.y,
            1.0f,
            D3DCOLOR_ARGB(vertices[i].color.a, vertices[i].color.r, vertices[i].color.g, vertices[i].color.b),
            vertices[i].texCoords.x / textureSize.x,
            vertices[i].texCoords.y / textureSize.y};

        repackedVertices.push_back(vertex);
    }

    // Create a vertex buffer to hold our vertices data
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    context->CreateVertexBuffer(vertexCount*sizeof(CUSTOMVERTEX), 0, CUSTOMFVF,
        D3DPOOL_MANAGED, &vertexBuffer,  NULL);

    // Copy our vertices into the vertex buffer
    VOID* pvoid;
    vertexBuffer->Lock(0, 0, (void**)&pvoid, 0);
    memcpy(pvoid, &repackedVertices[0], vertexCount*sizeof(CUSTOMVERTEX));
    vertexBuffer->Unlock();

    // Quads primitive isn't supported by DirectX, fall back to TrianglesFan primitive
    if (type == Quads)
        type = TrianglesFan;

    // TrianglesStrip primitive isn't supported by DirectX, fall back to TrianglesFan primitive
    if (type == TrianglesStrip)
        type = TrianglesFan;

    // Find the DirectX primitive type
    static const D3DPRIMITIVETYPE modes[] = {D3DPT_POINTLIST, D3DPT_LINELIST,
        D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, static_cast<D3DPRIMITIVETYPE>(0), D3DPT_TRIANGLEFAN};

    D3DPRIMITIVETYPE mode = modes[type];

    // Compute the number of primitives to draw
    unsigned int primitiveCount;

    if (mode == D3DPT_POINTLIST)
        primitiveCount = vertexCount;
    else if (mode == D3DPT_LINELIST || mode == D3DPT_LINESTRIP)
        primitiveCount = vertexCount - 1;
    else if (mode == D3DPT_TRIANGLELIST)
        primitiveCount = vertexCount / 3;
    else if (mode == D3DPT_TRIANGLEFAN)
        primitiveCount = vertexCount - 2;

    // Draw the primitives
    context->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
    context->DrawPrimitive(mode, 0, primitiveCount);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::pushStates()
{
    m_states->Capture();
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::popStates()
{
    m_states->Apply();
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::resetStates()
{
    IDirect3DDevice9* context = getContextHandle();

    // Define the default DirectX states
    context->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    context->SetRenderState(D3DRS_LIGHTING, FALSE);
    //context->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    //context->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    // Set the unique FVF that SFML uses
    context->SetFVF(CUSTOMFVF);

    // Set the view matrix
    D3DXMATRIX matrix;
    D3DXMatrixLookAtLH(&matrix, &D3DXVECTOR3(0.0f, 0.0f, 10.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    context->SetTransform(D3DTS_VIEW, &matrix);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::applyCurrentView(const View& view, IntRect& viewport, Vector2u& size)
{
    IDirect3DDevice9* context = getContextHandle();

    // Compute the viewport
    int top = size.y - (viewport.top + viewport.height);
    m_viewport.X = viewport.left;
    m_viewport.Y = top;
    m_viewport.Width = viewport.width;
    m_viewport.Height = viewport.height;
    m_viewport.MinZ = 0.f;
    m_viewport.MaxZ = 0.f;

    // Set the viewport
    context->SetViewport(&m_viewport);

    // Set the projection matrix
    D3DXMATRIX matrix(view.getTransform().getMatrix());
    context->SetTransform(D3DTS_PROJECTION, &matrix);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::applyBlendMode(BlendMode mode)
{
    // To implement
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::applyTransform(const Transform& transform)
{
    IDirect3DDevice9* context = getContextHandle();

    // Set the view matrix
    D3DXMATRIX matrix(transform.getMatrix());
    context->SetTransform(D3DTS_VIEW, &matrix);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDX::applyTexture(const Texture* texture)
{
    IDirect3DDevice9* context = getContextHandle();

    Texture::bind(texture, Texture::Pixels);
    context->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

    if (texture)
        textureSize = texture->getSize();
    else
        textureSize = Vector2u(1, 1);
}

} // namespace priv

} // namespace sf
