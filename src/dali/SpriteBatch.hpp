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
            
            /** @brief Adds a sprite into the batch.
             *  @param points The points forming the sprite.
             *  @param coords The texture coordinates of the sprite.*/
            void AddSprite( const Vector2f points[4], const Vector2f coords[2] );

        private:
            const Texture& m_Texture;
            std::vector<Vector2f>   m_Points; /**< @brief The vector of points.*/
            std::vector<Vector2f>   m_Coords; /**< @brief The vector of texture coordinates.*/
    };
}

#endif
