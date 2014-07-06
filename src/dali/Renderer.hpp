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
#include <GL/glew.h>
#include <SDL/SDL.h>

namespace dali {

    class Renderer {
        public:

            Renderer( const RendererConfig& config );
            ~Renderer();

            /** @brief Starts up the renderer.*/
            void            	StartUp();

            /** @brief Shuts down the renderer.*/
            void            	ShutDown();

            /** @brief Starts a new frame.*/
            void            	BeginFrame();

            /** @brief Ends the current frame.*/
            void            	EndFrame();
            /*                void            	DrawSprite(const SOGLSpriteInfo* spriteInfo);
                              void 				DrawSpriteBatch( const SOGLSpriteBatch* spriteBatch);
                              void            	DrawMesh(const SOGLMeshInfo* meshInfo);
                              void            	DrawBox( const SMatrix3x3* matrix, const SRGBAColor* color);
                              void            	DrawLight( const SOGLLightInfo* lightInfo );
                              void            	DrawText( const char_t * text, const uint32_t posX,
                              const uint32_t posY, const uint32_t size = 1);
                              void            	SetViewMatrix(const SMatrix3x3* viewMatrix);
                              SOGLRendererConfig 	GetRendererConfig();
                              */

        private:
            /**	@brief Initializes OpenGL.*/
            void InitOpenGL();

            /** @brief Sets the projection for the grid aligned rendering mode.*/
            void SetProjectionGridAligned();

            /** @brief Sets the projection for the grid aligned rendering mode.*/
            void SetProjectionPixelAligned();

            float   m_ProjectionMatrix[4][4];     
            float   m_ScreenMatrix[4][4];     

            RendererConfig  m_Config;

            ///**	@brief Renders the specified primitive.
            // *   @param renderingInfo The rendering info of the primitive to render.*/
            //void RenderDynamicSprite( const SOGLSpriteInfo* spriteInfo);

            ///**	@brief Renders the specified primitive.
            // *   @param renderingInfo The rendering info of the primitive to render.
            // *   @param shader The shader to use.*/
            //void RenderDynamicSprite( const SOGLSpriteInfo* spriteInfo, const SOGLShader* shader);

            ///** @brief Renders a box.
            // *	@param boxInfo The box to render.*/
            //void RenderBox( const SOGLBoxInfo* boxInfo);

            ///** @brief Renders a mesh.
            // *	@param meshInfo The mesh to render.*/
            //void RenderMesh( const SOGLMeshInfo* meshInfo);

            ///** @brief Renders a chunk of static sprites.
            // *	@param sprites The vector of sprites to render.
            // *	@param init The index of the begining of the chunk.
            // *	@param end The index of the end of the chunk.*/
            //void RenderChunk( const std::vector< const SOGLSpriteInfo* > sprites, uint32_t init, uint32_t end );

            ///**	@brief Sets the stencil buffer to one for the specified primitive.
            // *   @param renderingInfo The rendering info of the primitive to render.*/
            //void RenderStencil( const SOGLRenderingInfo* renderingInfo);

            ///**	@brief Renders the shadow of the specified object for the given light.
            // *   @param shadow The quad specifying the shadow to render.*/
            //void RenderShadow( const SQuad& shadow );

            ///** @brief Renders a textured quad.
            // *  @param quad The quad coordinates.
            // *  @param texCoords The texture coordinates.*/
            //void RenderTexturedQuad( const SVector2* quad, const SVector2* texCoords);

            ///** @brief Renders a colored quad.
            // *  @param quad The quad coordinates.
            // *  @param color The color.*/
            //void RenderColoredQuad( const SVector2* quad, const SRGBAColor& color );

            ///**	@brief Sets the rendering state.
            // *   @param spriteInfo The rendering info of the sprite to render.*/
            //void SetSpriteRenderingState( const SOGLSpriteInfo* spriteInfo );

            ///**	@brief Sets the rendering state.
            // *   @param boxInfo The rendering info of the box to render.*/
            //void SetBoxRenderingState( const SOGLBoxInfo* boxInfo );

            ///**	@brief Sets the rendering state.
            // *   @param lightInfo The light info from which the state have to be taken.*/
            //void SetRenderingState( const SOGLLightInfo* lightInfo );

            ///** @brief Sets the rendering state to draw the lighting texture.*/
            //void SetLightingBlendRenderingState();

            ///** @brief Sets the projection for the pixel aligned rendering mode.*/
            //void SetProjectionPixelAligned();


            ///** @brief Sets the GUI projection for the pixel aligned rendering mode.*/
            //void SetGUIProjectionPixelAligned();

            ///** @brief Sets the GUI projection for the grid aligned rendering mode.*/
            //void SetGUIProjectionGridAligned();

            ///** @brief This function renders the shadows into the stencil buffer.*/
            //void RenderShadows();

            ///** @brief This function performs the rendering of the scene to the scene texture.
            // *  @param b True to render ground. False otherwise.*/
            //void RenderScene( bool b);

            ///** @brief This function renders the static objects.*/
            //void RenderStaticObjects();

            ///** @brief This function renders the dynamic objects.*/
            //void RenderDynamicObjects();

            ///** @brief This function renders the sprite batches.*/
            //void RenderSpriteBatches();

            //void RenderLighting( const SOGLSpriteInfo* spriteInfo );

            ///** @brief This function performs the rendering of the debugging info.*/
            //void RenderDebug();

            ///** @brief This function performs the rendering of the GUI to the frame buffer.*/
            //void RenderGUI();

            ///** @brief This function blends the scene and the lighting to the scene buffer.*/
            //void BlendSceneAndLighting();

            ///** @brief Computes the shadow quads of those lights in falling inside the frustum.*/
            //void ComputeShadowQuads();

            ///** @brief Loads a texture*/
            //void LoadTexture( const char_t* textureName, SOGLTexture* texture );

            ///** @brief Loads a shader*/
            //void LoadShader( const char_t* shaderName, SOGLShader* shader );

            ///** @brief Unloads a textures.*/
            //void UnloadTexture( SOGLTexture* texture );

            ///** @brief Unloads a shader.*/
            //void UnloadShader( SOGLShader* shader );

            ////Shader satate functions
            //void ShaderSetProjectionMatrix( GLint pos );
            //void ShaderSetScreenMatrix( GLint pos );
            //void ShaderSetViewMatrix( GLint pos );
            //void ShaderSetModelMatrix( GLint pos );
            //void ShaderSetTexDiffuse( GLint pos );
            //void ShaderSetAmbient( GLint pos );
            //void ShaderSetLightPosition( GLint pos );
            //void ShaderSetLightRadius( GLint pos );
            //void SetQuad( const SVector2* quad );
            //void SetTexCoords( const SVector2* texCoords );

            //void ActivateShader( const SOGLShader* shader );

            ///** @brief Creates the shadow quad of a sprite.*/
            //void CreateShadowQuad( const SOGLRenderingInfo* renderingInfo, const SVector2& lightPosition, const float32_t radius, SQuad* shadowQuad);

            //bool NextChunk( SOGLRenderingInfo** vertexData, uint32_t* chunkSize );

            ////Configuration.
            //uint32_t			m_ViewportWidth;		/**< @brief The viewport width.*/
            //uint32_t			m_ViewportHeight;		/**< @brief The viewport height.*/
            //float32_t			m_ScreenUnitWidth;		/**< @brief The number of cells that fit on the screen width.*/
            //float32_t			m_ScreenUnitHeight;		/**< @brief The number of cells that fit on the screen height.*/
            //SDL_Surface*    		m_Screen;			/**< @brief The screen containing the opengl context.*/
            //SOGLRendererConfig		m_Config;			/**< @brief The configuration parameters of the renderer.*/


            ////Rendering state data	
            //float32_t     			m_ProjectionMatrix[4][4];     	/**< @brief An array storing the projection matrix.*/
            //float32_t				m_ScreenMatrix[4][4];         	/**< @brief An array storing the matrix used to scale the scene to the screen resolution.*/
            //float32_t				m_ViewMatrix[4][4];				/**< @brief An array storing the view matrix.*/
            //float32_t				m_Identity[4][4];	    		/**< @brief An array storing an identity matrix.*/
            //float32_t				m_GUIProjectionMatrix[4][4];	/**< @brief An array storing the projection matrix for the GUI.*/
            //float32_t				m_GUIScreenMatrix[4][4];		/**< @brief An array storing the matrix used to scale GUI to the screen resolution.*/
            //float32_t				m_ModelMatrix[4][4]; 			/**< @brief An array stroting the current model matrix.*/	
            //SOGLTexture*			m_TexDiffuse;					/**< @brief The current diffuse texture.*/
            //uint32_t				m_ActiveLayer;					/**< @brief The layer.*/
            //SRGBAColor				m_Ambient;						/**< @brief The ambient component of the scene.*/
            //float32_t				m_LightPosition[4];				/**< @brief The position of the currently activated light.*/

            //float32_t 				m_LightRadius;					/**< @brief The radius of the currently activated light.*/	



            ////Lighting rendering members.
            //GLuint		                m_LightingTexture;		/**< @brief The texture used for lighting.*/
            //SOGLLightInfo*  	        m_Lights;		      	/**< @brief The list of lights to render.*/
            //uint32_t					m_NumLights;            	/**< @brief The number of lights*/
            //uint32_t					m_MaxNumLights;        		/**< @brief The maximum number of lights*/
            //std::map<SOGLLightInfo*, std::vector<SQuad> > m_ShadowQuads;

            ////Scene rendering members.
            //GLuint			 				m_SceneTexture;    		/**< @brief The texture used for the scene.*/
            ////				SOGLRenderingInfo**				m_Layers;					  /**< @brief The list of primivites to render by layer.*/
            ////				uint32_t*                      	m_NumRenderingInfo;		/**< @brief The number of Rendering info per layer.*/
            ////				uint32_t*                      	m_MaxNumRenderingInfo;	 	/**< @brief The maximum number of Rendering info per layer.*/

            //std::vector<const SOGLSpriteInfo*> 		m_StaticSprites;	/**< @brief The list of static sprites to render.*/
            //std::vector<const SOGLSpriteInfo*> 		m_DynamicSprites;	/**< @brief The list of dynamic sprites to render.*/
            //std::vector<const SOGLSpriteBatch*>		m_SpriteBatches;	/**< @brief The list of sprite batches to render.*/
            //std::vector<SOGLBoxInfo> 				m_Boxes;			/**< @brief The list of boxes to render.*/
            //std::vector<SOGLMeshInfo>				m_Meshes;			/**< @brief The list of meshes to render.*/
            //std::vector<SOGLSpriteInfo> 			m_Gui;				/**< @brief The list of gui elements to render.*/

            ////Context change minimization variables
            //GLuint				m_CurrentShader;		/**< @brief The currently bound shader.*/
            //GLuint				m_CurrentTexture;		/**< @brief The currently texture 0 bound.*/

            ////Resources.
            //COGLShaderLoader*		m_ShaderLoader;				/**< @brief The shader loader.*/		
            //COGLTextureLoader*		m_TextureLoader;			/**< @brief The texture loader.*/
            //SOGLTexture             m_FontTexture;				/**< @brief The font texture.*/
            //SOGLShader          	m_FontShader;				/**< @brief The shader to render the fonts.*/
            //SOGLShader          	m_SimpleShader;				/**< @brief The simple shader to render a flat object.*/
            //SOGLShader          	m_SpriteShader;				/**< @brief The shader to render sprites.*/
            //SOGLShader          	m_AmbientShader;			/**< @brief The shader to render sprites with ambient light component.*/
            //SOGLShader          	m_ShadowShader;				/**< @brief The shader to render the shadows.*/
            //SOGLShader          	m_PointLightShader;			/**< @brief The shader to render spries with a point light source.*/

            //SQuad 					m_BasicQuad;
            //SVector2            	m_Quad[6];    				/**< @brief The quad used to render the quads to the screen.*/
            //SVector2				m_TextureCoords[6];			/**< @brief The texture coords to render.*/
            //std::vector<float32_t>	m_VertexBuffer;				/**< @brief The vertex buffer used to render static objects.*/	
            //std::vector<float32_t>	m_TextureBuffer;			/**< @brief The texture buffer used to render static objects.*/

            ////Shader state members
            //std::vector< std::function<void ( GLint )> > 	m_ShaderStateFunctions;	/**< @brief The vector containing the shader state functions.*/
    };
}
#endif
