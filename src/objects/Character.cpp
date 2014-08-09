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
#include <cstring>

namespace sacman {

    const char* Character::m_Type = "character";

    Character::Character( const char* name, const char* spriteName, const math::Vector2f& position, const math::Vector2f& extent ) :
        Entity( name ),
        m_SpriteRenderer( NULL ),  
        m_Box( "", E_DYNAMIC, E_SOLID, position, extent, this ), 
        m_CurrentState(E_STAND),
        m_PreviousState(E_RIGHT),
        m_IsGrounded(true)  {
            dali::Sprite* sprite = dali::spriteLoader.Load( spriteName );
            m_SpriteRenderer = new dali::SpriteRenderer( *sprite );

    }

    Character::~Character() {
            delete m_SpriteRenderer;
    }

    void Character::Draw( const double elapsedTime, const int depth ) const {
        math::Vector2f position = m_Box.Position();
        math::Vector2f extent = m_Box.Extent();
        m_SpriteRenderer->Draw( Context::m_Renderer, elapsedTime, depth, position, extent );
    }

    void Character::DrawShape( const double elapsedTime, const int depth ) const {
        m_Box.DrawShape(elapsedTime, depth);
    }

    void Character::Update( const double elapsedTime ) {
        switch(m_CurrentState) {
            case E_RIGHT:
                m_Box.Move( 3.0f );
                if( m_PreviousState != E_RIGHT ) {
                    m_SpriteRenderer->LaunchAnimation("WalkRight",0.5f,true);
                    m_PreviousState = E_RIGHT;
                }
                break;
            case E_LEFT:
                m_Box.Move( -3.0f );
                if( m_PreviousState != E_LEFT ) {
                    m_SpriteRenderer->LaunchAnimation("WalkLeft",0.5f,true);
                    m_PreviousState = E_LEFT;
                }
                break;
            case E_STAND:
                m_Box.Move( 0.0f ); 
                if( m_PreviousState == E_RIGHT ) {
                    m_SpriteRenderer->LaunchAnimation("StandRight",0.5f,true);
                    m_PreviousState = E_STAND;
                } else if( m_PreviousState == E_LEFT ) {
                    m_SpriteRenderer->LaunchAnimation("StandLeft",0.5f,true);
                    m_PreviousState = E_STAND;
                }
                break;
            default:
                break;
        }
    }

    void Character::Collide( const Collision& collision ) {
        if( std::strcmp(collision.m_Entity->Type(), "box" ) == 0 )  {
            m_IsGrounded = collision.m_Type == E_ENTER ? true : false;
            //std::cout << "IS GROUNDED: " << m_IsGrounded << std::endl;
        }
    }

    void Character::Enter( Level* level ) {
        m_Box.Enter( level );
        level->RegisterListener(KEYBOARD, std::bind(&Character::ListenKeyboard,this,std::placeholders::_1) );
    }

    void Character::Leave( Level* level ) {
        level->UnregisterListener(KEYBOARD, std::bind(&Character::ListenKeyboard,this,std::placeholders::_1) );
        m_Box.Leave( level );
    }

    math::Vector2f Character::Position() const {
        return m_Box.Position();
    }

    void Character::SetPosition( const math::Vector2f& position ) {
        m_Box.SetPosition( position );
    }

    math::Vector2f Character::Extent() const {
        return m_Box.Extent();
    }

    const char* Character::Type() const {
        return m_Type;
    }


    void Character::ListenKeyboard( std::shared_ptr<void> data ) {
        KeyEvent* event = static_cast<KeyEvent*>(data.get());
        if( event->m_KEType == K_PRESSED ) {
            switch( event->m_KCode ) {
                case K_RIGHT:
                    m_CurrentState = E_RIGHT;
                    break;
                case K_LEFT:
                    m_CurrentState = E_LEFT;
                    break;
                case K_SPACE:
                    if(m_IsGrounded) m_Box.ApplyForce({0.0f,8.0f});
                    break;
                default:
                    break;
            }
        } else {
            if( event->m_KCode == K_RIGHT || event->m_KCode == K_LEFT ) {
                m_PreviousState = m_CurrentState;
                m_CurrentState = E_STAND;
            }
        }
    }


}
