/*Sacman is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Sacman is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#ifndef SPRITE_H
#define SPRITE_H

#include "Types.hpp"
#include "Buffer.hpp"

namespace dali {
    class ResourceLibrary;
    class Texture;
    class Renderer;
    class Sprite {
        public:
            Sprite();
            ~Sprite();

            void Load( ResourceLibrary& library, const char* fileName );

            void Draw( Renderer& renderer, const Vector2f& position = {0.0f, 0.0f}, 
                       const Vector2f& scale = {2.0f, 2.0f} );

        private:
            Vector2fBuffer    m_Vertices;
            Vector2fBuffer    m_TexCoords;
            IndexBuffer       m_Indices;
            Texture*          m_Texture;
    };
}

#endif
