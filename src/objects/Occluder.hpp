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

#ifndef OCCLUDER_H
#define OCCLUDER_H

#include "objects/Entity.hpp"
#include "objects/Body.hpp"
#include "objects/StaticSprite.hpp"
#include "math/Types.hpp"

namespace sacman {
    class Occluder : public Entity {
        public:
            Occluder(const char* name, const char* textureName, const math::Vector2f texCoords[4] );
            ~Occluder();

            math::Vector2f Position() const ;
            math::Vector2f Extent() const ;
            void SetPosition( const math::Vector2f& position );
            const char* Type() const;
            void Draw(const double elapsedTime, const int depth) const;
            void DrawShape( const double elapsedTime, const int depth ) const;
            void Enter(Level* level, const math::Vector2f position, const math::Vector2f& extent);
            void Leave( Level* level );

        private:
            static const char*                  m_Type;
            Body                                m_Body;
            StaticSprite                        m_Sprite;
    };

}

#endif