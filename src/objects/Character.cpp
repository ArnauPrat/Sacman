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
    Character::Character( const char* name, Level& level, const char* spriteName, const math::Vector2f& position, const math::Vector2f& extent ) :
        Entity( name, level ),
        m_Position( position ),
        m_Scale( extent ),
        m_SpriteRenderer( NULL ),  
        m_Level( level ),
        m_Body( NULL ),
        m_Fixture( NULL ),
        m_CurrentState(E_STAND),
        m_PreviousState(E_RIGHT) {
            dali::Sprite* sprite = dali::spriteLoader.Load( spriteName );
            m_SpriteRenderer = new dali::SpriteRenderer( *sprite );

            /** Physics Stuff **/
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set( m_Position.m_X + 0.5f*m_Scale.m_X, m_Position.m_Y + 0.5f*m_Scale.m_Y);
            m_Body = m_Level.B2World().CreateBody(&bodyDef);
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(0.5f*m_Scale.m_X, 0.5f*m_Scale.m_Y);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            m_Fixture = m_Body->CreateFixture(&fixtureDef);
    }

    Character::~Character() {
            delete m_SpriteRenderer;
            m_Body->DestroyFixture( m_Fixture );
            m_Level.B2World().DestroyBody(m_Body);
    }

    void Character::Draw( const double elapsedTime, const int depth ) const {
            b2Vec2 position = m_Body->GetPosition();
            math::Vector2f drawPosition = { position.x - 0.5f*m_Scale.m_X, position.y - 0.5f*m_Scale.m_Y};
            m_SpriteRenderer->Draw( Context::m_Renderer, elapsedTime, depth, drawPosition, m_Scale );
    }

    void Character::DrawShape( const double elapsedTime, const int depth ) const {
            const b2AABB& aabb = m_Fixture->GetAABB(0);
            b2Vec2 center = aabb.GetCenter();
            b2Vec2 extents = aabb.GetExtents();
            Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , {1.0f, 0.0f, 0.0f, 1.0f}, 256 );
    }

    
    void Character::MoveRight( float speed ) {
        b2Vec2 vel = m_Body->GetLinearVelocity();
        float velChange = speed - vel.x;
        float impulse = m_Body->GetMass() * velChange;
        m_Body->ApplyLinearImpulse( b2Vec2(impulse,0), m_Body->GetWorldCenter(), true );
    }

    void Character::Stop() {
        b2Vec2 vel = m_Body->GetLinearVelocity();
        float velChange = 0.0f - vel.x;
        float impulse = m_Body->GetMass() * velChange;
        m_Body->ApplyLinearImpulse( b2Vec2(impulse,0), m_Body->GetWorldCenter(), true );
    }

    void Character::MoveLeft( float speed ) {
        b2Vec2 vel = m_Body->GetLinearVelocity();
        float velChange = -speed - vel.x;
        float impulse = m_Body->GetMass() * velChange;
        m_Body->ApplyLinearImpulse( b2Vec2(impulse,0), m_Body->GetWorldCenter(), true );
    }

    math::Vector2f Character::Position() const {
        assert(false);
    }

    void Character::SetPosition( const math::Vector2f& position ) {
        assert(false);
    }

    math::Vector2f Character::Extent() const {
        assert(false);
    }

    void Character::SetExtent( const math::Vector2f& extent ) {
        assert(false);
    }

    void Character::Update( const double elapsedTime ) {
        switch(m_CurrentState) {
            case E_RIGHT:
                MoveRight( 1.0f );
                if( m_PreviousState != E_RIGHT ) {
                    m_SpriteRenderer->LaunchAnimation("WalkRight",1.0f,true);
                    m_PreviousState = E_RIGHT;
                }
                break;
            case E_LEFT:
                MoveLeft( 1.0f );
                if( m_PreviousState != E_LEFT ) {
                    m_SpriteRenderer->LaunchAnimation("WalkLeft",1.0f,true);
                    m_PreviousState = E_LEFT;
                }
                break;
            case E_STAND:
                Stop();
                if( m_PreviousState == E_RIGHT ) {
                    m_SpriteRenderer->LaunchAnimation("StandRight",1.0f,true);
                    m_PreviousState = E_STAND;
                } else if( m_PreviousState == E_LEFT ) {
                    m_SpriteRenderer->LaunchAnimation("StandLeft",1.0f,true);
                    m_PreviousState = E_STAND;
                }
                break;
            default:
                break;
        }
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
                default:
                    break;
            }
        } else {
            m_PreviousState = m_CurrentState;
            m_CurrentState = E_STAND;
        }
    }

}
