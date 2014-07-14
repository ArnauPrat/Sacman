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

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "Texture.hpp"
#include "Types.hpp"
#include <vector>

namespace dali {
    class SpriteBatch {
        friend class Renderer;
        public:
            SpriteBatch( const Texture& texture );
            ~SpriteBatch();
            
            /** @brief Adds tex quads into the batch.
             *  @param texQuads The tex quads forming the sprites.
             *  @param numQuads The number of tex quads.*/
            void AddTexQuads( const TexQuad texQuads[], const int numQuads );

        private:
            const Texture&      m_Texture;
            TexQuad*            m_TexQuads;   /**< @brief The tex quads forming the sprite batch.*/
    };
}

#endif
