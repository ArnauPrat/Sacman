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

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Box2D/Box2D.h"
#include "dali/Dali.hpp"
#include "dali/SpriteRenderer.hpp"
#include "math/Types.hpp"
#include "objects/Entity.hpp"
#include <memory>


namespace sacman {
    class Level;
    enum MovementState {
        E_RIGHT,
        E_LEFT,
        E_STAND
    };
    class Character : public Entity {
        public:
            Character( const char* name, Level& level, const char* spriteName, const math::Vector2f& position, const math::Vector2f& extent );
            ~Character();

            void Draw( const double elapsedTime, const int depth ) const ;

            void DrawShape( const double elapsedTime, const int depth ) const;

            void Update( const double elapsedTime );

            void ListenKeyboard( std::shared_ptr<void> data );

            void MoveRight( float speed );
            void MoveLeft( float speed );
            void Stop();

            virtual math::Vector2f Position() const;
            virtual void SetPosition( const math::Vector2f& position );
            virtual math::Vector2f Extent() const;
            virtual void SetExtent( const math::Vector2f& extent );

        private:
            math::Vector2f          m_Position;
            math::Vector2f          m_Scale;
            dali::SpriteRenderer*   m_SpriteRenderer;
            Level&                  m_Level;
            b2Body*                 m_Body;
            b2Fixture*              m_Fixture;
            MovementState           m_CurrentState;
            MovementState           m_PreviousState;
    };
}


#endif
