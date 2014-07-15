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
#include <functional>

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

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer() {
	}

	void Renderer::StartUp( const RendererConfig& config ) {
		m_Config = config;

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
			// Draw
		}
	}

	void Renderer::DrawSpriteBach( const SpriteBatch& spriteBatch ) {
		DrawingInfo info;
		info.m_Vertices = spriteBatch.m_Vertices;
		info.m_TexCoords = spriteBatch.m_TexCoords;
		info.m_Indices = spriteBatch.m_Indices;
		info.m_NumQuads = spriteBatch.m_NumQuads;
		info.m_Texture = spriteBatch.m_Texture.m_TextureID;
		m_FrameDrawingInfo.push_back(info);

	}	

	void Renderer::InitOpenGL() {
		glViewport( 0, 0, m_Config.m_ViewportWidth, m_Config.m_ViewportHeight );
		glClearColor(0.0f, 0.0f, 0.0f,1.0f);                   
		glClearDepth(1);                         
		glDepthFunc(GL_LEQUAL);   
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST); 
		glEnable(GL_STENCIL_TEST);
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
		glUniformMatrix3fv( pos, 1, GL_FALSE, static_cast<const GLfloat*>(&m_ModelMatrix[0][0]) );
	}

	void Renderer::ShaderSetTexDiffuse( GLint pos ) {
	/*	glActiveTexture(GL_TEXTURE0);
		if( m_CurrentTexture != m_TexDiffuse->texID ) {
			glBindTexture(GL_TEXTURE_2D, m_TexDiffuse->texID);
			glUniform1i(pos, 0);
			m_CurrentTexture = m_TexDiffuse->texID;
		}    
		*/
	}

}
