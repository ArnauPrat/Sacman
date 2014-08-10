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

#ifndef SACMAN_BODY_H
#define SACMAN_BODY_H

#include "Level.hpp"
#include "Entity.hpp"
#include "math/Types.hpp"
#include <Box2D/Box2D.h>

namespace sacman {

    enum BodyType{
        E_STATIC,
        E_DYNAMIC
    };

    enum BoxType {
        E_SOLID,
        E_SENSOR
    };

    class Body : public Entity {
        public:
            Body( const char* name, 
                 const BodyType type,
                 const math::Vector2f& position
                 );

            ~Body();

            math::Vector2f Position() const ;
            math::Vector2f Extent() const ;
            void SetPosition( const math::Vector2f& position );
            const char* Type() const;
            void DrawShape( const double elapsedTime, const int depth ) const;
            void Enter( Level* level );
            void Leave( Level* level );
            void Move( const float speed );
            void ApplyForce( const math::Vector2f& force );
            void AddBox( const math::Vector2f& position, 
                         const math::Vector2f extent, 
                         const BoxType boxType,
                         void* userData = NULL );


        private:
            static const char*                  m_Type;
            b2Body*                             m_Body;
            std::vector<b2Fixture*>             m_Fixtures;
            BodyType                            m_BodyType;
            math::Vector2f                      m_Position;
    };
}

#endif

