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


#include "Character.hpp"
#include "Context.hpp"

namespace sacman {
    Character::Character() :
     m_SpriteRenderer( NULL ) {
         m_Position = {0.0f, 0.0f};
         m_Scale = {2.0f, 2.0f};
    }

    Character::~Character() {
        if( m_SpriteRenderer != NULL ) {
            delete m_SpriteRenderer;
        }
    }

    void Character::Load( const char* spriteName ) {
        dali::Sprite* sprite = dali::spriteLoader.Load( spriteName );
        m_SpriteRenderer = new dali::SpriteRenderer( *sprite );
    }

    void Character::Draw( double elapsedTime ) {
        m_SpriteRenderer->Draw( Context::m_Renderer, elapsedTime, m_Position, m_Scale );
    }
    void Character::LaunchAnimation( const char* name, double totalTime, bool loop  ) {
        m_SpriteRenderer->LaunchAnimation( name, totalTime, loop );
    }

    void Character::StopAnimation() {
        m_SpriteRenderer->StopAnimation();
    }

    bool Character::IsAnimationRunning( const char* name ) {
        return m_SpriteRenderer->IsAnimationRunning( name );
    }
}
