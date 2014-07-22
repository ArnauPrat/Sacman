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
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>

namespace dali {

	static void SetIdentity( float matrix[3][3] ) {
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

	Renderer::Renderer() :
		m_Textured(NULL),
        m_Quad( GL_INVALID_VALUE )
	{
	}

	Renderer::~Renderer() {
        if( m_Quad != GL_INVALID_VALUE ) {
            glDeleteBuffers(1, &m_Quad );
        }
	}

	void Renderer::StartUp( const RendererConfig& config ) {
		m_Config = config;
        PrintConfig( m_Config );

		/** Initialize Projection matrix. */
		switch( m_Config.m_RenderingMode ) {
			case PIXEL_ALIGNED:
				SetProjectionPixelAligned();
				break;
			case GRID_ALIGNED:
				SetProjectionGridAligned();
				break;
		}

		/** Initialize matrices to identity.*/
		SetIdentity( m_ViewMatrix );
		SetIdentity( m_ModelMatrix );

		/** Setting current rendering data.*/
		m_CurrentInfo = NULL;

		InitOpenGL();

		/** Set Shader state functions .*/
		Effect::SetStateFunction( E_PROJECTION_MATRIX, std::bind(&Renderer::ShaderSetProjectionMatrix, this, std::placeholders::_1 ));
		Effect::SetStateFunction( E_VIEW_MATRIX, std::bind(&Renderer::ShaderSetViewMatrix, this, std::placeholders::_1 ));
		Effect::SetStateFunction( E_MODEL_MATRIX, std::bind(&Renderer::ShaderSetModelMatrix, this, std::placeholders::_1 ));
		Effect::SetStateFunction( E_TEX_DIFFUSE, std::bind(&Renderer::ShaderSetTexDiffuse, this, std::placeholders::_1 ));

		m_Textured = m_EffectLoader.Load("./effects/E_Sprite.sha");

        /** Creating Quad buffer. **/
        glGenBuffers(1, &m_Quad);
        assert( m_Quad != GL_INVALID_VALUE );
        float tempBuffer[8];
        tempBuffer[0] = 0.0;
        tempBuffer[1] = 0.0;
        tempBuffer[2] = 1.0;
        tempBuffer[3] = 0.0;
        tempBuffer[4] = 1.0;
        tempBuffer[5] = 1.0;
        tempBuffer[6] = 0.0;
        tempBuffer[7] = 1.0;
        
        glBindBuffer( GL_ARRAY_BUFFER, m_Quad );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float )*4, tempBuffer, GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

	void Renderer::ShutDown() {
	}

	void Renderer::BeginFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		m_FrameDrawingInfo.clear();
	}

	void Renderer::EndFrame() {
		int size = m_FrameDrawingInfo.size();
		for( int i = 0; i < size; ++i ) {
            m_CurrentInfo = &m_FrameDrawingInfo[i];
            // Set Effect
            Effect::SetEffect(*m_Textured);
            // Draw
            glEnableClientState(GL_VERTEX_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, m_CurrentInfo->m_Vertices);
            glVertexPointer(2,GL_FLOAT,0,0);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, m_CurrentInfo->m_TexCoords);
            glTexCoordPointer(2,GL_FLOAT,0,0);

            glEnableClientState(GL_INDEX_ARRAY);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CurrentInfo->m_Indices);
            glDrawElements(GL_TRIANGLES,m_CurrentInfo->m_NumIndices,GL_UNSIGNED_SHORT,0);
            glDisableClientState(GL_INDEX_ARRAY);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		}
	}

    void    Renderer::Draw( const Vector2fBuffer& vertices, 
            const Vector2fBuffer& texCoords,
            const IndexBuffer& indices,
            const Texture& texture,
            const Vector2f& translate, 
            const Vector2f& scale) {

        DrawingInfo info;
        info.m_Vertices = vertices.m_Data;
        info.m_TexCoords = texCoords.m_Data;
        info.m_Indices = indices.m_Data;
        info.m_Texture = texture.m_TextureID;
        info.m_NumVertices = vertices.m_NumElements;
        info.m_NumTexCoords = texCoords.m_NumElements;
        info.m_NumIndices = indices.m_NumElements;
        info.m_Translate = translate;
        info.m_Scale = scale;
        m_FrameDrawingInfo.push_back( info );
    }

	void Renderer::InitOpenGL() {
		GLuint err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			std::cout << "ERROR Loading glew" << std::endl;
		}
		std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		glViewport( 0, 0, m_Config.m_ViewportWidth, m_Config.m_ViewportHeight );
		glClearColor(0.0f, 0.0f, 1.0f,1.0f);                   
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST); 
		glDisable(GL_STENCIL_TEST); 
	}

	void Renderer::SetProjectionGridAligned() {
		float bottom = 0;
		float left = 0;
		float right = m_Config.m_GridWidth;
		float top = m_Config.m_GridHeight;

		m_ProjectionMatrix[0][0] = 2/(float)(right-left);
		m_ProjectionMatrix[0][1] = 0;
		m_ProjectionMatrix[0][2] = 0;

		m_ProjectionMatrix[1][0] = 0;
		m_ProjectionMatrix[1][1] = 2/(float)(top-bottom);
		m_ProjectionMatrix[1][2] = 0;

		m_ProjectionMatrix[2][0] = -(right+left)/(float)(right-left);
		m_ProjectionMatrix[2][1] = -(top+bottom)/(float)(top-bottom);
		m_ProjectionMatrix[2][2] = 1 ;
	}

	void Renderer::SetProjectionPixelAligned() {
		float bottom = 0;
		float left = 0;
		float right = m_Config.m_ViewportWidth;
		float top = m_Config.m_ViewportHeight;

		m_ProjectionMatrix[0][0] = 2/(float)(right-left) * m_Config.m_CellWidth;;
		m_ProjectionMatrix[0][1] = 0;
		m_ProjectionMatrix[0][2] = 0;

		m_ProjectionMatrix[1][0] = 0;
		m_ProjectionMatrix[1][1] = 2/(float)(top-bottom) * m_Config.m_CellHeight;
		m_ProjectionMatrix[1][2] = 0;

		m_ProjectionMatrix[2][0] = -(right+left)/(float)(right-left);
		m_ProjectionMatrix[2][1] = -(top+bottom)/(float)(top-bottom);
		m_ProjectionMatrix[2][3] = 1;
	}

	void Renderer::ShaderSetProjectionMatrix( GLint pos ) {
		glUniformMatrix3fv( pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ProjectionMatrix[0][0]) );
	}

	void Renderer::ShaderSetViewMatrix( GLint pos ) {
		glUniformMatrix3fv( pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ViewMatrix[0][0]) );
	}

	void Renderer::ShaderSetModelMatrix( GLint pos ) {
        std::memset(&m_ModelMatrix[0][0],0,sizeof(float)*9);
        m_ModelMatrix[2][0] = m_CurrentInfo->m_Translate.m_X;
        m_ModelMatrix[2][1] = m_CurrentInfo->m_Translate.m_Y;
        m_ModelMatrix[2][2] = 1.0f;
        m_ModelMatrix[0][0] = m_CurrentInfo->m_Scale.m_X;
        m_ModelMatrix[1][1] = m_CurrentInfo->m_Scale.m_Y;
		glUniformMatrix3fv( pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ModelMatrix[0][0]) );
	}

	void Renderer::ShaderSetTexDiffuse( GLint pos ) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_CurrentInfo->m_Texture);
		glUniform1i(pos, 0);
	}
}
