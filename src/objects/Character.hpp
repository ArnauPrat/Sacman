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

#ifndef SACMANCHARACTER_H
#define SACMANCHARACTER_H

#include "Box2D/Box2D.h"
#include "arnitech/renderer/SpriteRenderer.hpp"
#include "arnitech/math/Types.hpp"
#include "arnitech/scene/Body.hpp"
#include "arnitech/scene/Entity.hpp"
#include <memory>

class atLevel;
struct MovementState {
    bool m_Right : 1;
    bool m_Left : 1;
    bool m_Jump : 1;
    bool m_Stand : 1;
    bool m_Occluder : 1;
};

class Character : public atEntity {
    public:
        Character( const char* name, const char* spriteName );
        ~Character();

        void Draw( const double elapsedTime, const int depth ) const ;
        void DrawShape( const double elapsedTime, const int depth ) const;
        void Update( const double elapsedTime );
        void Collide( const atCollision& collision );
        void Enter( atLevel* level, const atVector2f position, const atVector2f& extent);
        void Leave( atLevel* level );


        void MoveRight( float speed );
        void MoveLeft( float speed );
        void Stop();

        virtual atVector2f Position() const;
        virtual void SetPosition( const atVector2f& position );
        virtual atVector2f Extent() const;
        virtual const char* Type() const;

    private:
        static const char* m_Type;
        void ListenKeyboard( std::shared_ptr<void> data );

        atSpriteRenderer*       m_SpriteRenderer;
        atBody                  m_Body;
        MovementState           m_CurrentState;
        MovementState           m_PreviousState;
        bool                    m_IsGrounded;
        atVector2f              m_Extent;
        int                     m_OccluderDepth;
};

#endif
