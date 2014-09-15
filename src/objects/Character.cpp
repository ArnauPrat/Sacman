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
#include "arnitech/system/Context.hpp"
#include "arnitech/resources/Resources.hpp"
#include <cstring>


const char* Character::m_Type = "character";

Character::Character( const char* name, 
        const char* spriteName) :
    atEntity( name ),
    m_SpriteRenderer( NULL ),  
    m_Body( "", E_DYNAMIC ), 
    m_IsGrounded(true),
    m_OccluderDepth(0)
{
    atSprite* sprite = spriteLoader.Load( spriteName );
    m_SpriteRenderer = new atSpriteRenderer( *sprite );
    std::memset(&m_CurrentState, 0, sizeof(MovementState));
    m_CurrentState.m_Stand = true;
    std::memset(&m_PreviousState, 0, sizeof(MovementState));
    m_PreviousState.m_Stand = true;
}

Character::~Character() {
    delete m_SpriteRenderer;
}

void Character::Draw( const double elapsedTime, const int depth ) const {
    atVector2f position = m_Body.Position();
    atVector2f extent = m_Extent;
    position.m_X -= extent.m_X;
    position.m_Y -= extent.m_Y;
    extent.m_X *=2.0f;
    extent.m_Y *=2.0f;
    m_SpriteRenderer->Draw( atContext::m_Renderer, elapsedTime, depth, position, extent );
}

void Character::DrawShape( const double elapsedTime, const int depth ) const {
    m_Body.DrawShape(elapsedTime, depth);
}

void Character::Update( const double elapsedTime ) {
    if( m_CurrentState.m_Right ) {
        m_Body.Move( 5.0f );
    }

    if( (m_CurrentState.m_Left) != 0) {
        m_Body.Move( -5.0f );
    }
    if( (m_CurrentState.m_Stand ) != 0) {
        m_Body.Move( 0.0f ); 
    }
}

void Character::Collide( const atCollision& collision ) {

    if( std::strcmp(collision.m_Entity->Type(), "body" ) == 0 )  {
        m_IsGrounded = collision.m_Type == E_ENTER ? true : false;
        if( m_IsGrounded ) m_CurrentState.m_Jump = false;
        return;
    }

    if (std::strcmp(collision.m_Entity->Type(), "occluder") == 0)  {
        if (collision.m_Type == E_ENTER) {
            m_OccluderDepth = collision.m_Entity->Depth();
            m_CurrentState.m_Occluder = true;
        }

        if (collision.m_Type == E_LEAVE){
            if (!(m_CurrentState.m_Occluder)) SetDepth(m_OccluderDepth + 1);
            m_CurrentState.m_Occluder = false;
        }
        return;
    }
}

void Character::Enter( atLevel* level, const atVector2f position, const atVector2f& extent) {
    m_Extent = extent;
    m_Body.Enter( level, position, extent );
    m_Body.AddBox( {0.0f, 0.0f}, m_Extent, E_SOLID );  
    m_Body.AddBox( {0.0f, -m_Extent.m_Y}, { 0.3f, 0.1f }, E_SENSOR, this );  
    level->RegisterListener( E_KEYBOARD, std::bind(&Character::ListenKeyboard,this,std::placeholders::_1) );
}

void Character::Leave( atLevel* level ) {
    level->UnregisterListener(E_KEYBOARD, std::bind(&Character::ListenKeyboard,this,std::placeholders::_1) );
    m_Body.Leave( level );
}

atVector2f Character::Position() const {
    return m_Body.Position();
}

void Character::SetPosition( const atVector2f& position ) {
    m_Body.SetPosition( position );
}

atVector2f Character::Extent() const {
    return m_Body.Extent();
}

const char* Character::Type() const {
    return m_Type;
}


void Character::ListenKeyboard( std::shared_ptr<void> data ) {
    atKeyEvent* event = static_cast<atKeyEvent*>(data.get());
    if( event->m_KEType == K_PRESSED ) {
        switch( event->m_KCode ) {
            case E_K_RIGHT:
                if( !m_CurrentState.m_Right ) {
                    m_SpriteRenderer->LaunchAnimation("WalkRight",0.5f,true);
                }
                m_CurrentState.m_Right = true; 
                m_CurrentState.m_Left = false;
                m_CurrentState.m_Stand = false;
                break;
            case E_K_LEFT:
                if( !m_CurrentState.m_Left ) {
                    m_SpriteRenderer->LaunchAnimation("WalkLeft",0.5f,true);
                }
                m_CurrentState.m_Left = true;
                m_CurrentState.m_Right = false;
                m_CurrentState.m_Stand = false;
                break;
            case E_K_SPACE:
                if(m_IsGrounded && !m_CurrentState.m_Jump) {
                    m_Body.ApplyForce({0.0f,9.0f});
                    m_CurrentState.m_Jump = true;
                }
                break;
            case E_K_E:
                if (m_CurrentState.m_Occluder) {
                    SetDepth(m_OccluderDepth - 1);
                    m_CurrentState.m_Occluder = false;
                }
                break;
            default:
                break;
        }
    } else {
        if( event->m_KCode == E_K_RIGHT || event->m_KCode == E_K_LEFT ) {
            m_PreviousState = m_CurrentState;

            if( m_PreviousState.m_Right )
                m_SpriteRenderer->LaunchAnimation("StandRight",0.5f,true);

            if( m_PreviousState.m_Left ) 
                m_SpriteRenderer->LaunchAnimation("StandLeft",0.5f,true);

            m_CurrentState.m_Right = false;
            m_CurrentState.m_Left = false ;
            m_CurrentState.m_Stand = true;
        }
    }
}

