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
#include "Globals.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <algorithm>
#include <FreeImage.h>

namespace dali {

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

    Renderer::Renderer()
    {
    }

    Renderer::~Renderer() {
    }

    void Renderer::StartUp(const RendererConfig& config) {
        FreeImage_Initialise();
        m_Config = config;
        PrintConfig(m_Config);

        /** Initialize Projection matrix. */
        switch (m_Config.m_RenderingMode) {
        case PIXEL_ALIGNED:
            SetProjectionPixelAligned();
            break;
        case GRID_ALIGNED:
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
        Effect::SetStateFunction(E_PROJECTION_MATRIX, std::bind(&Renderer::ShaderSetProjectionMatrix, this, std::placeholders::_1));
        Effect::SetStateFunction(E_VIEW_MATRIX, std::bind(&Renderer::ShaderSetViewMatrix, this, std::placeholders::_1));
        Effect::SetStateFunction(E_MODEL_MATRIX, std::bind(&Renderer::ShaderSetModelMatrix, this, std::placeholders::_1));
        Effect::SetStateFunction(E_TEX_DIFFUSE, std::bind(&Renderer::ShaderSetTexDiffuse, this, std::placeholders::_1));
        Effect::SetStateFunction(E_COLOR_DIFFUSE, std::bind(&Renderer::ShaderSetColorDiffuse, this, std::placeholders::_1));

        m_Textured = effectLoader.Load("E_Sprite.sha");
        m_Flat = effectLoader.Load("E_Flat.sha");

        /** Creating Quad buffer. **/
        math::Vector2f data[4];
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

    void Renderer::ShutDown() {
        FreeImage_DeInitialise();
    }

    void Renderer::BeginFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_FrameDrawingInfo.clear();
    }

    bool Renderer::SortByDepth(const Renderer::DrawingInfo& a, const Renderer::DrawingInfo& b) {
        return a.m_Depth < b.m_Depth;
    }

    void Renderer::EndFrame() {
        std::sort(m_FrameDrawingInfo.begin(), m_FrameDrawingInfo.end(), SortByDepth);
        int size = m_FrameDrawingInfo.size();
        for (int i = 0; i < size; ++i) {
            m_CurrentInfo = &m_FrameDrawingInfo[i];
            glPolygonMode(GL_FRONT, m_CurrentInfo->m_PolygonMode);
            Effect::SetEffect(*m_CurrentInfo->m_Effect);
            //glEnableClientState(GL_VERTEX_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, m_CurrentInfo->m_Vertices->m_Data);
//            glVertexPointer(2, GL_FLOAT, 0, 0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            //glEnableClientState(GL_INDEX_ARRAY);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CurrentInfo->m_Indices->m_Data);
            glDrawElements(GL_TRIANGLES, m_CurrentInfo->m_Indices->m_NumElements, GL_UNSIGNED_SHORT, 0);

            //glDisableClientState(GL_INDEX_ARRAY);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);
            //glDisableClientState(GL_VERTEX_ARRAY);
            //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    void    Renderer::Draw(const Vector2fBuffer* vertices,
        const Vector2fBuffer* texCoords,
        const IndexBuffer* indices,
        const Texture* texture,
        const int depth,
        const void* texOffset,
        const math::Vector2f& translate,
        const math::Vector2f& scale) {
        DrawingInfo info;
        info.m_Vertices = const_cast<Vector2fBuffer*>(vertices);
        info.m_TexCoords = const_cast<Vector2fBuffer*>(texCoords);
        info.m_Indices = const_cast<IndexBuffer*>(indices);
        info.m_Texture = const_cast<Texture*>(texture);
        info.m_Effect = m_Textured;
        info.m_Depth = depth;
        info.m_TexOffset = const_cast<void*>(texOffset);
        info.m_Translate = translate;
        info.m_Scale = scale;
        info.m_PolygonMode = GL_FILL;
        m_FrameDrawingInfo.push_back(info);
    }

    void    Renderer::Draw(const Vector2fBuffer* texCoords,
        const Texture* texture,
        const int depth,
        const math::Vector2f& translate,
        const math::Vector2f& scale) {
        DrawingInfo info;
        info.m_Vertices = &m_Quad;
        info.m_TexCoords = const_cast<Vector2fBuffer*>(texCoords);
        info.m_Indices = &m_Indices;
        info.m_Texture = const_cast<Texture*>(texture);
        info.m_Effect = m_Textured;
        info.m_Depth = depth;
        info.m_TexOffset = static_cast<void*>(0);
        info.m_Translate = translate;
        info.m_Scale = scale;
        info.m_PolygonMode = GL_FILL;
        m_FrameDrawingInfo.push_back(info);
    }

    void    Renderer::DrawBox(const math::Vector2f& min,
        const math::Vector2f& extent,
        const RGBAColor& color,
        const int depth) {
        DrawingInfo info;
        info.m_Vertices = &m_Quad;
        info.m_Indices = &m_Indices;
        info.m_ColorDiffuse = color;
        info.m_Effect = m_Flat;
        info.m_Translate = min;
        info.m_Scale = extent;
        info.m_TexOffset = static_cast<void*>(0);
        info.m_Depth = depth;
        info.m_PolygonMode = GL_LINE;
        m_FrameDrawingInfo.push_back(info);
    }

    void    Renderer::SetCameraPosition(const math::Vector2f& position) {
        m_ViewMatrix[0][0] = 1.0f;
        m_ViewMatrix[0][1] = 0.0f;
        m_ViewMatrix[0][2] = 0.0f;

        m_ViewMatrix[1][0] = 0.0f;
        m_ViewMatrix[1][1] = 1.0f;
        m_ViewMatrix[1][2] = 0.0f;

        m_ViewMatrix[2][0] = -position.m_X;
        m_ViewMatrix[2][1] = -position.m_Y;
        m_ViewMatrix[2][2] = 0.0f;
    }


    void Renderer::InitOpenGL() {
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

    void Renderer::SetProjectionGridAligned() {
        float bottom = 0;
        float left = 0;
        float right = m_Config.m_GridWidth;
        float top = m_Config.m_GridHeight;

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

    void Renderer::SetProjectionPixelAligned() {
        float bottom = 0;
        float left = 0;
        float right = m_Config.m_ViewportWidth;
        float top = m_Config.m_ViewportHeight;

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

    void Renderer::ShaderSetProjectionMatrix(GLint pos) {
        glUniformMatrix3fv(pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ProjectionMatrix[0][0]));
    }

    void Renderer::ShaderSetViewMatrix(GLint pos) {
        glUniformMatrix3fv(pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ViewMatrix[0][0]));
    }

    void Renderer::ShaderSetModelMatrix(GLint pos) {
        std::memset(&m_ModelMatrix[0][0], 0, sizeof(float)* 9);
        m_ModelMatrix[2][0] = m_CurrentInfo->m_Translate.m_X;
        m_ModelMatrix[2][1] = m_CurrentInfo->m_Translate.m_Y;
        m_ModelMatrix[2][2] = 1.0f;
        m_ModelMatrix[0][0] = m_CurrentInfo->m_Scale.m_X;
        m_ModelMatrix[1][1] = m_CurrentInfo->m_Scale.m_Y;
        glUniformMatrix3fv(pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ModelMatrix[0][0]));
    }

    void Renderer::ShaderSetTexDiffuse(GLint pos) {
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, m_CurrentInfo->m_TexCoords->m_Data);
        //glTexCoordPointer(2, GL_FLOAT, 0, static_cast<const GLvoid*>(m_CurrentInfo->m_TexOffset));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,static_cast<const GLvoid*>(m_CurrentInfo->m_TexOffset));
        glEnableVertexAttribArray(1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_CurrentInfo->m_Texture->m_TextureID);
        glUniform1i(pos, 0);
    }

    void Renderer::ShaderSetColorDiffuse(GLint pos) {
        glUniform4f(pos,
            m_CurrentInfo->m_ColorDiffuse.m_R,
            m_CurrentInfo->m_ColorDiffuse.m_G,
            m_CurrentInfo->m_ColorDiffuse.m_B,
            m_CurrentInfo->m_ColorDiffuse.m_A);
    }
}
