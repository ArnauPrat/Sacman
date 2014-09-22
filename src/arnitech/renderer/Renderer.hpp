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

#ifndef ATRENDERER_H
#define ATRENDERER_H

#include "Buffer.hpp"
#include "Effect.hpp"
#include "RendererConfig.hpp"
#include "Texture.hpp"
#include "Types.hpp"
#include "common/Macros.hpp"
#include <GL/glew.h>
#include <math/Types.hpp>
#include <vector>


class atRenderer {
    ATNON_COPYABLE(atRenderer)

    struct atDrawingInfo {
        atVertexBuffer*       m_Vertices;
        atTexCoordBuffer*     m_TexCoords;
        atIndexBuffer*        m_Indices;
        atTexture*            m_Texture;
        void*               m_TexOffset;
        atEffect*             m_Effect;
        int                 m_Depth;
        atVector2f      m_Translate;
        atVector2f      m_Scale;
        atRGBAColor     m_ColorDiffuse;
        GLenum              m_PolygonMode; 
    };

    public:

    atRenderer();
    ~atRenderer();

    /** @brief Starts up the renderer.
     *  @param config The structure holding the configuration of the renderer.*/
    ATEXPORT void            	StartUp( const atRendererConfig& config );

    /** @brief Shuts down the renderer.*/
    ATEXPORT void            	ShutDown();

    /** @brief Starts a new frame.*/
    ATEXPORT void            	BeginFrame();

    /** @brief Ends the current frame.*/
    ATEXPORT void            	EndFrame();


    ATEXPORT void                Draw(   const atVertexBuffer* vertices, 
                                const atTexCoordBuffer* texCoords,
                                const atIndexBuffer* indices,
                                const atTexture* texture,
                                const int depth,
                                const void* texOffset,
                                const atVector2f& translate = {0.0f, 0.0f}, 
                                const atVector2f& scale = {1.0f, 1.0f} );

    ATEXPORT void                Draw(   const atTexCoordBuffer* texCoords,
                                const atTexture* texture,
                                const int depth,
                                const atVector2f& translate = {0.0f, 0.0f}, 
                                const atVector2f& scale = {1.0f, 1.0f} );

    ATEXPORT void                DrawBox( const atVector2f& min, 
                                 const atVector2f& extent,
                                 const atRGBAColor& color,
                                 const int depth );

    ATEXPORT void                SetCameraPosition( const atVector2f& position );


    private:

    /** @brief Used to sort drawing info structs.*/
    static bool                 SortByDepth( const atDrawingInfo& a, const atDrawingInfo& b );

    /** @brief Tests if a Drawinginfo is inside the frustrum **/
    bool                        InsideFrustrum(const atDrawingInfo& info);

    /**	@brief Initializes OpenGL.*/
    void                        InitOpenGL();

    /** @brief Sets the projection for the grid aligned rendering mode.*/
    void                        SetProjectionGridAligned();

    /** @brief Sets the projection for the grid aligned rendering mode.*/
    void                        SetProjectionPixelAligned();

    /** Matrices.*/
    float                       m_ProjectionMatrix[3][3];       /**< @brief The projection matrix used to render.*/
    float                       m_ViewMatrix[3][3];             /**< @brief The current view matrix.*/
    float                       m_ModelMatrix[3][3];            /**< @brief The current model matrix.*/

    /** Frustrum limits **/
    atVector2f                  m_FrustrumMin;
    atVector2f                  m_FrustrumMax;
    atVector2f                  m_FrustrumCenter;

    /** Current rendering data */
    atDrawingInfo*	            m_CurrentInfo;	    /**< @brief The current texture to use for rendering.*/
    std::vector<atDrawingInfo>  m_FrameDrawingInfo;	/** @brief The drawing info objects to render this frame.*/

    /** Configuration. */
    atRendererConfig            m_Config;

    /** Resources. */
    atEffect*			        m_Textured;         /**< @brief The effect used to render textured quads.*/
    atEffect*                   m_Flat;             /**< @brief The effect used to print shapes with flat color such as boxes.*/
    atVertexBuffer              m_Quad;             /**< @brief The buffer id of a buffer containing a quad.*/
    atIndexBuffer               m_Indices;          /**< @brief The buffer id of a buffer containing indices.*/

    /** Shader satate functions */
    void                        ShaderSetProjectionMatrix( GLint pos );
    void                        ShaderSetViewMatrix( GLint pos );
    void                        ShaderSetModelMatrix( GLint pos );
    void                        ShaderSetTexDiffuse( GLint pos );
    void                        ShaderSetColorDiffuse( GLint pos);
};
#endif
