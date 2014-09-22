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

#ifndef ATSPRITE_H
#define ATSPRITE_H

#include "common/Platform.hpp"
#include "Buffer.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "common/Macros.hpp"
#include "math/Types.hpp"
#include <vector>

#define ATANIMATION_NAME_LENGTH 32
#define ATANIMATION_NUM_FRAMES 16
class atSprite {
    ATNON_COPYABLE(atSprite)
        friend class atSpriteRenderer;
    public:

    struct atAnimation{
        char     m_Name[ATANIMATION_NAME_LENGTH];
        char     m_Frames[ATANIMATION_NUM_FRAMES];
        char     m_NumFrames;
    };

    ATEXPORT atSprite();
    ATEXPORT ~atSprite();

    ATEXPORT void Load( const char* fileName );

    private:
    static bool                 m_Initialized;
    static atVertexBuffer       m_Vertices;
    static atIndexBuffer        m_Indices;
    atTexCoordBuffer            m_TexCoords;
    atTexture*                  m_Texture;
    std::vector<atAnimation>    m_Animations;
};

#endif
