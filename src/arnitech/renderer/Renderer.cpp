/*Dali is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Dali is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.*/


#include "Effect.hpp"
#include "Renderer.hpp"
#include "resources/Resources.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <algorithm>
#include <FreeImage.h>


static void SetIdentity(float matrix[3][3]) {
    matrix[0][0] = 1.0f;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;

    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f;
    matrix[1][2] = 0.0f;

    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
}

atRenderer::atRenderer()
{
}

atRenderer::~atRenderer() {
}

void atRenderer::StartUp(const atRendererConfig& config) {
    FreeImage_Initialise();
    m_Config = config;
    PrintConfig(m_Config);

    /** Initialize Projection matrix. */
    switch (m_Config.m_RenderingMode) {
        case E_PIXEL_ALIGNED:
            SetProjectionPixelAligned();
            break;
        case E_GRID_ALIGNED:
            SetProjectionGridAligned();
            break;
    }

    /** Initialize matrices to identity.*/
    SetIdentity(m_ViewMatrix);
    SetIdentity(m_ModelMatrix);

    /** Setting current rendering data.*/
    m_CurrentInfo = NULL;

    InitOpenGL();

    /** Set Shader state functions .*/
    atEffect::SetStateFunction(E_PROJECTION_MATRIX, std::bind(&atRenderer::ShaderSetProjectionMatrix, this, std::placeholders::_1));
    atEffect::SetStateFunction(E_VIEW_MATRIX, std::bind(&atRenderer::ShaderSetViewMatrix, this, std::placeholders::_1));
    atEffect::SetStateFunction(E_MODEL_MATRIX, std::bind(&atRenderer::ShaderSetModelMatrix, this, std::placeholders::_1));
    atEffect::SetStateFunction(E_TEX_DIFFUSE, std::bind(&atRenderer::ShaderSetTexDiffuse, this, std::placeholders::_1));
    atEffect::SetStateFunction(E_COLOR_DIFFUSE, std::bind(&atRenderer::ShaderSetColorDiffuse, this, std::placeholders::_1));

    m_Textured = effectLoader.Load("E_Sprite.sha");
    m_Flat = effectLoader.Load("E_Flat.sha");

    /** Creating Quad buffer. **/
    atVector2f data[4];
    data[0].m_X = 0.0f;
    data[0].m_Y = 0.0f;
    data[1].m_X = 1.0f;
    data[1].m_Y = 0.0f;
    data[2].m_X = 1.0f;
    data[2].m_Y = 1.0f;
    data[3].m_X = 0.0f;
    data[3].m_Y = 1.0f;
    m_Quad.AddData(data, 4);

    unsigned short indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 3;
    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;
    m_Indices.AddData(indices, 6);
}

void atRenderer::ShutDown() {
    FreeImage_DeInitialise();
}

void atRenderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_FrameDrawingInfo.clear();
}

bool atRenderer::SortByDepth(const atDrawingInfo& a, const atDrawingInfo& b) {
    return a.m_Depth < b.m_Depth;
}

bool atRenderer::InsideFrustrum(const atDrawingInfo& info) {
    atVector2f realMin = { info.m_Vertices->m_AABBMin.m_X * info.m_Scale.m_X + info.m_Translate.m_X,
        info.m_Vertices->m_AABBMin.m_Y*info.m_Scale.m_Y + info.m_Translate.m_Y };
    atVector2f realMax = { info.m_Vertices->m_AABBMax.m_X * info.m_Scale.m_X + info.m_Translate.m_X,
        info.m_Vertices->m_AABBMax.m_Y*info.m_Scale.m_Y + info.m_Translate.m_Y };
    atVector2f realCenter = { (realMax.m_X - realMin.m_X) / 2.0f + realMin.m_X, (realMax.m_Y - realMin.m_Y) / 2.0f + realMin.m_Y };
    float tx = std::abs(realCenter.m_X - m_FrustrumCenter.m_X);
    if (tx > std::abs((realCenter.m_X - realMin.m_X) + (m_FrustrumCenter.m_X - m_FrustrumMin.m_X))) {
        return false;
    }

    float ty = std::abs(realCenter.m_Y - m_FrustrumCenter.m_Y);
    if (ty > std::abs((realCenter.m_Y - realMin.m_Y) + (m_FrustrumCenter.m_Y - m_FrustrumMin.m_Y))) {
        return false;
    }
    return true;
}


void atRenderer::EndFrame() {
    std::sort(m_FrameDrawingInfo.begin(), m_FrameDrawingInfo.end(), SortByDepth);
    int size = static_cast<int>(m_FrameDrawingInfo.size());
    int numDrawn = 0;
    for (int i = 0; i < size; ++i) {
        m_CurrentInfo = &m_FrameDrawingInfo[i];
        if (InsideFrustrum(*m_CurrentInfo)) {
            numDrawn++;
            glPolygonMode(GL_FRONT, m_CurrentInfo->m_PolygonMode);
            atEffect::SetEffect(*m_CurrentInfo->m_Effect);
            glBindBuffer(GL_ARRAY_BUFFER, m_CurrentInfo->m_Vertices->m_Data);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CurrentInfo->m_Indices->m_Data);
            glDrawElements(GL_TRIANGLES, m_CurrentInfo->m_Indices->m_NumElements, GL_UNSIGNED_SHORT, 0);

            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);
        }
    }
}

void    atRenderer::Draw(const atVertexBuffer* vertices,
                         const atVector2fBuffer* texCoords,
                         const atIndexBuffer* indices,
                         const atTexture* texture,
                         const int depth,
                         const void* texOffset,
                         const atVector2f& translate,
                         const atVector2f& scale) {
    atDrawingInfo info;
    info.m_Vertices     = const_cast<atVertexBuffer*>(vertices);
    info.m_TexCoords    = const_cast<atTexCoordBuffer*>(texCoords);
    info.m_Indices      = const_cast<atIndexBuffer*>(indices);
    info.m_Texture      = const_cast<atTexture*>(texture);
    info.m_Effect       = m_Textured;
    info.m_Depth        = depth;
    info.m_TexOffset    = const_cast<void*>(texOffset);
    info.m_Translate    = translate;
    info.m_Scale        = scale;
    info.m_PolygonMode  = GL_FILL;
    m_FrameDrawingInfo.push_back(info);
}

void    atRenderer::Draw(const atTexCoordBuffer* texCoords,
                         const atTexture* texture,
                         const int depth,
                         const atVector2f& translate,
                         const atVector2f& scale) {

    atDrawingInfo info;
    info.m_Vertices     = &m_Quad;
    info.m_TexCoords    = const_cast<atTexCoordBuffer*>(texCoords);
    info.m_Indices      = &m_Indices;
    info.m_Texture      = const_cast<atTexture*>(texture);
    info.m_Effect       = m_Textured;
    info.m_Depth        = depth;
    info.m_TexOffset    = static_cast<void*>(0);
    info.m_Translate    = translate;
    info.m_Scale        = scale;
    info.m_PolygonMode  = GL_FILL;
    m_FrameDrawingInfo.push_back(info);
}

void    atRenderer::DrawBox(const atVector2f& min,
                            const atVector2f& extent,
                            const atRGBAColor& color,
                            const int depth) {
    atDrawingInfo info;
    info.m_Vertices     = &m_Quad;
    info.m_Indices      = &m_Indices;
    info.m_ColorDiffuse = color;
    info.m_Effect       = m_Flat;
    info.m_Translate    = min;
    info.m_Scale        = extent;
    info.m_TexOffset    = static_cast<void*>(0);
    info.m_Depth        = depth;
    info.m_PolygonMode  = GL_LINE;
    m_FrameDrawingInfo.push_back(info);
}

void    atRenderer::SetCameraPosition(const atVector2f& position) {

    m_ViewMatrix[0][0] = 1.0f;
    m_ViewMatrix[0][1] = 0.0f;
    m_ViewMatrix[0][2] = 0.0f;

    m_ViewMatrix[1][0] = 0.0f;
    m_ViewMatrix[1][1] = 1.0f;
    m_ViewMatrix[1][2] = 0.0f;

    m_ViewMatrix[2][0] = -position.m_X;
    m_ViewMatrix[2][1] = -position.m_Y;
    m_ViewMatrix[2][2] = 0.0f;

    m_FrustrumCenter = { position.m_X, position.m_Y };
    if (m_Config.m_RenderingMode == E_GRID_ALIGNED) {
        m_FrustrumMin = { position.m_X - m_Config.m_GridWidth / 2.0f,
            position.m_Y - m_Config.m_GridHeight / 2.0f };
        m_FrustrumMax = { position.m_X + m_Config.m_GridWidth / 2.0f,
            position.m_Y + m_Config.m_GridHeight / 2.0f };
    }
    else {
        m_FrustrumMin = { position.m_X - m_Config.m_ViewportWidth / (2.0f*m_Config.m_CellWidth),
            position.m_Y - m_Config.m_ViewportHeight / (2.0f*m_Config.m_CellHeight) };
        m_FrustrumMax = { position.m_X + m_Config.m_ViewportWidth / (2.0f*m_Config.m_CellWidth),
            position.m_Y + m_Config.m_ViewportHeight / (2.0f*m_Config.m_CellHeight) };
    }
}

void atRenderer::InitOpenGL() {
    GLuint err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << "ERROR Loading glew" << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    glViewport(0, 0, m_Config.m_ViewportWidth, m_Config.m_ViewportHeight);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

void atRenderer::SetProjectionGridAligned() {
    float bottom = 0;
    float left = 0;
    float right = static_cast<float>(m_Config.m_GridWidth);
    float top = static_cast<float>(m_Config.m_GridHeight);

    m_ProjectionMatrix[0][0] = 2 / (float)(right - left);
    m_ProjectionMatrix[0][1] = 0;
    m_ProjectionMatrix[0][2] = 0;

    m_ProjectionMatrix[1][0] = 0;
    m_ProjectionMatrix[1][1] = 2 / (float)(top - bottom);
    m_ProjectionMatrix[1][2] = 0;

    m_ProjectionMatrix[2][0] = -(right + left) / (float)(right - left);
    m_ProjectionMatrix[2][1] = -(top + bottom) / (float)(top - bottom);
    m_ProjectionMatrix[2][2] = 1;
}

void atRenderer::SetProjectionPixelAligned() {
    float bottom = 0;
    float left = 0;
    float right = static_cast<float>(m_Config.m_ViewportWidth);
    float top = static_cast<float>(m_Config.m_ViewportHeight);

    m_ProjectionMatrix[0][0] = 2 / (float)(right - left) * m_Config.m_CellWidth;;
    m_ProjectionMatrix[0][1] = 0;
    m_ProjectionMatrix[0][2] = 0;

    m_ProjectionMatrix[1][0] = 0;
    m_ProjectionMatrix[1][1] = 2 / (float)(top - bottom) * m_Config.m_CellHeight;
    m_ProjectionMatrix[1][2] = 0;

    m_ProjectionMatrix[2][0] = -(right + left) / (float)(right - left);
    m_ProjectionMatrix[2][1] = -(top + bottom) / (float)(top - bottom);
    m_ProjectionMatrix[2][3] = 1;
}

void atRenderer::ShaderSetProjectionMatrix(GLint pos) {
    glUniformMatrix3fv(pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ProjectionMatrix[0][0]));
}

void atRenderer::ShaderSetViewMatrix(GLint pos) {
    glUniformMatrix3fv(pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ViewMatrix[0][0]));
}

void atRenderer::ShaderSetModelMatrix(GLint pos) {
    std::memset(&m_ModelMatrix[0][0], 0, sizeof(float)* 9);
    m_ModelMatrix[2][0] = m_CurrentInfo->m_Translate.m_X;
    m_ModelMatrix[2][1] = m_CurrentInfo->m_Translate.m_Y;
    m_ModelMatrix[2][2] = 1.0f;
    m_ModelMatrix[0][0] = m_CurrentInfo->m_Scale.m_X;
    m_ModelMatrix[1][1] = m_CurrentInfo->m_Scale.m_Y;
    glUniformMatrix3fv(pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ModelMatrix[0][0]));
}

void atRenderer::ShaderSetTexDiffuse(GLint pos) {
    glBindBuffer(GL_ARRAY_BUFFER, m_CurrentInfo->m_TexCoords->m_Data);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,static_cast<const GLvoid*>(m_CurrentInfo->m_TexOffset));
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CurrentInfo->m_Texture->m_TextureID);
    glUniform1i(pos, 0);
}

void atRenderer::ShaderSetColorDiffuse(GLint pos) {
    glUniform4f(pos,
            m_CurrentInfo->m_ColorDiffuse.m_R,
            m_CurrentInfo->m_ColorDiffuse.m_G,
            m_CurrentInfo->m_ColorDiffuse.m_B,
            m_CurrentInfo->m_ColorDiffuse.m_A);
}
