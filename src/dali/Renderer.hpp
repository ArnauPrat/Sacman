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

#ifndef RENDERER_H
#define RENDERER_H

#include "RendererConfig.hpp"
#include "SpriteBatch.hpp"
#include <GL/glew.h>
#include <vector>

namespace dali {
    class Renderer {

	    struct DrawingInfo {
		    GLuint m_Vertices;
		    GLuint m_TexCoords;
		    GLuint m_Indices;
		    GLuint m_Texture;
		    short  m_NumQuads;
	    };

        public:

            Renderer();
            ~Renderer();

            /** @brief Starts up the renderer.
             *  @param config The structure holding the configuration of the renderer.*/
            void            	StartUp( const RendererConfig& config );

            /** @brief Shuts down the renderer.*/
            void            	ShutDown();

            /** @brief Starts a new frame.*/
            void            	BeginFrame();

            /** @brief Ends the current frame.*/
            void            	EndFrame();

            /** @brief Draws a sprite batch.
             *  @param spriteBatch The sprite batch to draw.*/
            void                DrawSpriteBach( const SpriteBatch& spriteBatch );

        private:
            /**	@brief Initializes OpenGL.*/
            void InitOpenGL();

            /** @brief Sets the projection for the grid aligned rendering mode.*/
            void SetProjectionGridAligned();

            /** @brief Sets the projection for the grid aligned rendering mode.*/
            void SetProjectionPixelAligned();

	    /** Matrices.*/
	    float   m_ProjectionMatrix[3][3];       /**< @brief The projection matrix used to render.*/
	    float   m_ViewMatrix[3][3];             /**< @brief The current view matrix.*/
	    float   m_ModelMatrix[3][3];            /**< @brief The current model matrix.*/

	    /** Current rendering data */
	    DrawingInfo*	m_CurrentInfo;	    /**< @brief The current texture to use for rendering.*/
	    std::vector<DrawingInfo> m_FrameDrawingInfo;	/** @brief The drawing info objects to render this frame.*/

            /** Configuration. */
            RendererConfig  m_Config;

            /** Shader satate functions */
            void ShaderSetProjectionMatrix( GLint pos );
            void ShaderSetViewMatrix( GLint pos );
            void ShaderSetModelMatrix( GLint pos );
            void ShaderSetTexDiffuse( GLint pos );
    };
}
#endif
