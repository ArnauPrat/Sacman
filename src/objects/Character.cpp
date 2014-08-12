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

    Character::Character( const char* name, 
                          const char* spriteName, 
                          const math::Vector2f& position, 
                          const math::Vector2f& extent ) :
        Entity( name ),
        m_SpriteRenderer( NULL ),  
        m_Body( "", E_DYNAMIC, {position.m_X, position.m_Y} ), 
        m_CurrentState(E_STAND),
        m_PreviousState(E_RIGHT),
        m_IsGrounded(true),
        m_Extent( extent )  {
            dali::Sprite* sprite = dali::spriteLoader.Load( spriteName );
            m_SpriteRenderer = new dali::SpriteRenderer( *sprite );

    }

    Character::~Character() {
            delete m_SpriteRenderer;
    }

    void Character::Draw( const double elapsedTime, const int depth ) const {
        math::Vector2f position = m_Body.Position();
        math::Vector2f extent = m_Body.Extent();
        std::cout << extent.m_X << " " << extent.m_Y << std::endl;
        position.m_X -= extent.m_X;
        position.m_Y -= extent.m_Y;
        extent.m_X *=2.0f;
        extent.m_Y *=2.0f;
        m_SpriteRenderer->Draw( Context::m_Renderer, elapsedTime, depth, position, extent );
    }

    void Character::DrawShape( const double elapsedTime, const int depth ) const {
        m_Body.DrawShape(elapsedTime, depth);
    }

    void Character::Update( const double elapsedTime ) {
        if( (m_CurrentState & E_RIGHT) != 0) {
                m_Body.Move( 5.0f );
        }

        if( (m_CurrentState & E_LEFT) != 0) {
                m_Body.Move( -5.0f );
        }
        if( (m_CurrentState & E_STAND) != 0) {
                m_Body.Move( 0.0f ); 
        }
    }

    void Character::Collide( const Collision& collision ) {
        if( std::strcmp(collision.m_Entity->Type(), "body" ) == 0 )  {
            m_IsGrounded = collision.m_Type == E_ENTER ? true : false;
            if( m_IsGrounded ) m_CurrentState &= ~E_JUMP;
//            std::cout << "IS GROUNDED: " << m_IsGrounded << std::endl;
        }
    }

    void Character::Enter( Level* level ) {
        m_Body.Enter( level );
        m_Body.AddBox( {0.0f, 0.0f}, m_Extent, E_SOLID );  
        m_Body.AddBox( {0.0f, -m_Extent.m_Y}, { 0.3f, 0.1f }, E_SENSOR, this );  
        level->RegisterListener(KEYBOARD, std::bind(&Character::ListenKeyboard,this,std::placeholders::_1) );
    }

    void Character::Leave( Level* level ) {
        level->UnregisterListener(KEYBOARD, std::bind(&Character::ListenKeyboard,this,std::placeholders::_1) );
        m_Body.Leave( level );
    }

    math::Vector2f Character::Position() const {
        return m_Body.Position();
    }

    void Character::SetPosition( const math::Vector2f& position ) {
        m_Body.SetPosition( position );
    }

    math::Vector2f Character::Extent() const {
        return m_Body.Extent();
    }

    const char* Character::Type() const {
        return m_Type;
    }


    void Character::ListenKeyboard( std::shared_ptr<void> data ) {
        KeyEvent* event = static_cast<KeyEvent*>(data.get());
        if( event->m_KEType == K_PRESSED ) {
            switch( event->m_KCode ) {
                case K_RIGHT:
                    if( (m_CurrentState & E_RIGHT) == 0 ) {
                        m_SpriteRenderer->LaunchAnimation("WalkRight",0.5f,true);
                    }
                    m_CurrentState |= E_RIGHT;
                    m_CurrentState &= ~E_LEFT;
                    m_CurrentState &= ~E_STAND;
                    break;
                case K_LEFT:
                    if( (m_CurrentState & E_LEFT) == 0 ) {
                        m_SpriteRenderer->LaunchAnimation("WalkLeft",0.5f,true);
                    }
                    m_CurrentState |= E_LEFT;
                    m_CurrentState &= ~E_RIGHT;
                    m_CurrentState &= ~E_STAND;
                    break;
                case K_SPACE:
                    if(m_IsGrounded && ((m_CurrentState & E_JUMP) == 0 )) {
                        m_Body.ApplyForce({0.0f,9.0f});
                        m_CurrentState |= E_JUMP;
                    }
                    break;
                default:
                    break;
            }
        } else {
            if( event->m_KCode == K_RIGHT || event->m_KCode == K_LEFT ) {
                m_PreviousState = m_CurrentState;

                if( (m_PreviousState & E_RIGHT) != 0 )
                    m_SpriteRenderer->LaunchAnimation("StandRight",0.5f,true);

                if( (m_PreviousState & E_LEFT) != 0 ) 
                    m_SpriteRenderer->LaunchAnimation("StandLeft",0.5f,true);

                m_CurrentState &= ~E_RIGHT;
                m_CurrentState &= ~E_LEFT;
                m_CurrentState |= E_STAND;
            }
        }
    }


}
