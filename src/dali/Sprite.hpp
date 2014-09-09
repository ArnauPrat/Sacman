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

#ifndef SPRITE_H
#define SPRITE_H

#include "math/Types.hpp"
#include "dali/Buffer.hpp"
#include "dali/Texture.hpp"
#include "dali/Renderer.hpp"
#include <vector>

namespace dali {
#define ANIMATION_NAME_LENGTH 32
#define ANIMATION_NUM_FRAMES 16
    class Sprite {
        friend class SpriteRenderer;
        public:

            struct Animation{
                char     m_Name[ANIMATION_NAME_LENGTH];
                char     m_Frames[ANIMATION_NUM_FRAMES];
                char     m_NumFrames;
            };

            Sprite();
            ~Sprite();

            void Load( const char* fileName );


        private:
            static bool              m_Initialized;
            static VertexBuffer      m_Vertices;
            static IndexBuffer       m_Indices;
            TexCoordBuffer           m_TexCoords;
            Texture*                 m_Texture;
            std::vector<Animation> m_Animations;
    };
}

#endif
