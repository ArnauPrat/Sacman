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

#include "Buffer.hpp"
#include "Effect.hpp"
#include "RendererConfig.hpp"
#include "Texture.hpp"
#include <math/Types.hpp>
#include <GL/glew.h>
#include <vector>

namespace dali {

	class Renderer {

		struct DrawingInfo {
			GLuint m_Vertices;
			GLuint m_TexCoords;
			GLuint m_Indices;
			GLuint m_Texture;
            int    m_NumVertices;
            int    m_NumTexCoords;
            int    m_NumIndices;
            void*    m_TexOffset;
            math::Vector2f m_Translate;
            math::Vector2f m_Scale;
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
		 *  @param spriteBatch The sprite batch to draw.
         *  @param translate The translation to apply to the drawn sprite.
         *  @param scale The translation to apply to the drawn sprite.*/
		void                Draw( const Vector2fBuffer& vertices, 
                                  const Vector2fBuffer& texCoords,
                                  const IndexBuffer& indices,
                                  const Texture& texture,
                                  const void* texOffset,
                                  const math::Vector2f& translate = {0.0f, 0.0f}, 
                                  const math::Vector2f& scale = {1.0f, 1.0f} );

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

		/** Resources. */
		Effect			        m_Textured;         /**< @brief The effect used to render textured quads.*/
        GLuint                  m_Quad;             /**< @brief The buffer id of a buffer containing a quad.*/

		/** Shader satate functions */
		void ShaderSetProjectionMatrix( GLint pos );
		void ShaderSetViewMatrix( GLint pos );
		void ShaderSetModelMatrix( GLint pos );
		void ShaderSetTexDiffuse( GLint pos );
	};
}
#endif
