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
    Character::Character( Level& level ) :
     m_SpriteRenderer( NULL ),  
     m_Level( level ),
     m_Body( NULL ),
     m_Fixture( NULL ) {
         m_Position = {8.0f, 8.0f};
         m_Scale = {2.0f, 2.0f};
    }

    Character::~Character() {
        if( m_SpriteRenderer != NULL ) {
            delete m_SpriteRenderer;
        }

        if( m_Fixture ) {
            m_Body->DestroyFixture( m_Fixture );
        }

        if( m_Body ) {
            m_Level.B2World().DestroyBody(m_Body);
        }
    }

    void Character::Load( const char* spriteName ) {
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

    void Character::Draw( const double elapsedTime, const int depth ) const {
        if( m_SpriteRenderer ) {
            b2Vec2 position = m_Body->GetPosition();
            math::Vector2f drawPosition = { position.x - 0.5f*m_Scale.m_X, position.y - 0.5f*m_Scale.m_Y};
            m_SpriteRenderer->Draw( Context::m_Renderer, elapsedTime, depth, drawPosition, m_Scale );
        }
    }

    void Character::DrawShape() const  {
        if( m_Fixture ) {
            const b2AABB& aabb = m_Fixture->GetAABB(0);
            b2Vec2 center = aabb.GetCenter();
            b2Vec2 extents = aabb.GetExtents();
            Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , {1.0f, 0.0f, 0.0f, 1.0f} );
        }
    }

    void Character::LaunchAnimation( const char* name, const double totalTime, const bool loop  ) {
        m_SpriteRenderer->LaunchAnimation( name, totalTime, loop );
    }

    void Character::StopAnimation() {
        m_SpriteRenderer->StopAnimation();
    }

    bool Character::IsAnimationRunning( const char* name ) {
        return m_SpriteRenderer->IsAnimationRunning( name );
    }
}
