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

#ifndef BACKGROUND_HPP 
#define BACKGROUND_HPP 

#include "dali/Buffer.hpp"
#include "dali/Texture.hpp"
#include <vector>
#include "Entity.hpp"

namespace sacman {
    struct TiledLevel;
    class Background : public Entity {

        struct Chunk {
            dali::Vector2fBuffer m_Vertices;
            dali::Vector2fBuffer m_TexCoords;
            dali::IndexBuffer    m_Indices;
            dali::Texture*       m_Texture;
            int                  m_Depth;
            math::Vector2f       m_Min;
            math::Vector2f       m_Max;
        };

        public:
            Background( const char* name );
            ~Background();
            
            void Load( const TiledLevel& tiledLevel );

            void Draw( const double elapsedTime, const int depth );

            math::Vector2f Position() const;
            void SetPosition( const math::Vector2f& position );
            math::Vector2f Extent() const;
            const char* Type() const;

        private:
            static const char* m_Type;
            std::vector<Chunk*> m_Chunks;
            math::Vector2f      m_Min;
            math::Vector2f      m_Max;
    };
}

#endif
