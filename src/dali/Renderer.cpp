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


#include "Renderer.hpp"

namespace dali {

    Renderer::Renderer(const RendererConfig& config) :
           m_Config( config )
    {
    }

    Renderer::~Renderer() {
    }

    void Renderer::StartUp() {
    }

    void Renderer::ShutDown() {
    }

    void Renderer::BeginFrame() {
    }

    void Renderer::EndFrame() {
    }

    void Renderer::InitOpenGL()
    {
        //m_Config.m_ViewportHeight = m_Config.m_ViewportHeight > 0 ? m_Config.m_ViewportHeight : 1;
        //glViewport( 0, 0, m_Config.m_ViewportWidth, m_Config.m_ViewportHeight );
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
			m_ProjectionMatrix[0][3] = 0;

			m_ProjectionMatrix[1][0] = 0;
			m_ProjectionMatrix[1][1] = 2/(float)(top-bottom);
			m_ProjectionMatrix[1][2] = 0;
			m_ProjectionMatrix[1][3] = 0;

			m_ProjectionMatrix[2][0] = 0;
			m_ProjectionMatrix[2][1] = 0;
			m_ProjectionMatrix[2][2] = -2/(float)(1-(-1));
			m_ProjectionMatrix[2][3] = 0;

			m_ProjectionMatrix[3][0] = -(right+left)/(float)(right-left);
			m_ProjectionMatrix[3][1] = -(top+bottom)/(float)(top-bottom);
			m_ProjectionMatrix[3][2] = -(1+(-1))/(float)(1-(-1));
			m_ProjectionMatrix[3][3] = 1;

			m_ScreenMatrix[0][0] = 1;
			m_ScreenMatrix[0][1] = 0;
			m_ScreenMatrix[0][2] = 0;
			m_ScreenMatrix[0][3] = 0;

			m_ScreenMatrix[1][0] = 0;
			m_ScreenMatrix[1][1] = 1;
			m_ScreenMatrix[1][2] = 0;
			m_ScreenMatrix[1][3] = 0;

			m_ScreenMatrix[2][0] = 0;
			m_ScreenMatrix[2][1] = 0;
			m_ScreenMatrix[2][2] = 1;
			m_ScreenMatrix[2][3] = 0;

			m_ScreenMatrix[3][0] = 0;
			m_ScreenMatrix[3][1] = 0;
			m_ScreenMatrix[3][2] = 0;
			m_ScreenMatrix[3][3] = 1;
    }

    void Renderer::SetProjectionPixelAligned() {
			float bottom = 0;
			float left = 0;
			float right = m_Config.m_ViewportWidth;
			float top = m_Config.m_ViewportHeight;

			m_ProjectionMatrix[0][0] = 2/(float)(right-left);
			m_ProjectionMatrix[0][1] = 0;
			m_ProjectionMatrix[0][2] = 0;
			m_ProjectionMatrix[0][3] = 0;

			m_ProjectionMatrix[1][0] = 0;
			m_ProjectionMatrix[1][1] = 2/(float)(top-bottom);
			m_ProjectionMatrix[1][2] = 0;
			m_ProjectionMatrix[1][3] = 0;

			m_ProjectionMatrix[2][0] = 0;
			m_ProjectionMatrix[2][1] = 0;
			m_ProjectionMatrix[2][2] = -2/(float)(1-(-1));
			m_ProjectionMatrix[2][3] = 0;

			m_ProjectionMatrix[3][0] = -(right+left)/(float)(right-left);
			m_ProjectionMatrix[3][1] = -(top+bottom)/(float)(top-bottom);
			m_ProjectionMatrix[3][2] = -(1+(-1))/(float)(1-(-1));
			m_ProjectionMatrix[3][3] = 1;

			m_ScreenMatrix[0][0] = m_Config.m_CellWidth;
			m_ScreenMatrix[0][1] = 0;
			m_ScreenMatrix[0][2] = 0;
			m_ScreenMatrix[0][3] = 0;

			m_ScreenMatrix[1][0] = 0;
			m_ScreenMatrix[1][1] = m_Config.m_CellHeight;
			m_ScreenMatrix[1][2] = 0;
			m_ScreenMatrix[1][3] = 0;

			m_ScreenMatrix[2][0] = 0;
			m_ScreenMatrix[2][1] = 0;
			m_ScreenMatrix[2][2] = 1;
			m_ScreenMatrix[2][3] = 0;

			m_ScreenMatrix[3][0] = 0;
			m_ScreenMatrix[3][1] = 0;
			m_ScreenMatrix[3][2] = 0;
			m_ScreenMatrix[3][3] = 1;
    }
}
