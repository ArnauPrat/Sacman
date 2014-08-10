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

#include "Body.hpp"
#include "Context.hpp"

namespace sacman {

    const char* Body::m_Type = "body";

    Body::Body( const char* name,
            const BodyType bodyType,
            const math::Vector2f& position ) :
        Entity( name ),
        m_BodyType( bodyType )
    {
        m_Position = position;
    }

    Body::~Body() {
    }

    void Body::Enter( Level* level ) {
        b2BodyDef bodyDef;
        switch(m_BodyType) {
            case E_DYNAMIC:
                bodyDef.type = b2_dynamicBody;
                bodyDef.fixedRotation = true;
                break;
            case E_STATIC:
                break;
        }

        bodyDef.position.Set( m_Position.m_X, m_Position.m_Y );
        m_Body = level->B2World().CreateBody(&bodyDef);
    }

    void Body::Leave( Level* level ) {
        for( b2Fixture* fixture : m_Fixtures ) {
            m_Body->DestroyFixture( fixture );
        }
        level->B2World().DestroyBody(m_Body);
    }

    math::Vector2f Body::Position() const {
        b2Vec2 position = m_Body->GetPosition();
        return { position.x, position.y}; 
    }

    math::Vector2f Body::Extent() const {
        b2AABB aabb;
        for( b2Fixture* fixture : m_Fixtures ) {
            aabb.Combine(fixture->GetAABB(0));
        }
        b2Vec2 extents = aabb.GetExtents();
        return {extents.x, extents.y};
    }

    const char* Body::Type() const {
        return m_Type;
    }

    void Body::SetPosition( const math::Vector2f& position ) {
        m_Body->SetTransform( b2Vec2(position.m_X, position.m_Y), 0.0f);
    }

    void Body::DrawShape( const double elapsedTime, const int depth ) const {
        for( b2Fixture* fixture : m_Fixtures ) {
            const b2AABB& aabb = fixture->GetAABB(0);
            b2Vec2 center = aabb.GetCenter();
            b2Vec2 extents = aabb.GetExtents();
            dali::RGBAColor color;
            if(fixture->IsSensor()) {
                color =  {0.0f, 0.0f, 1.0f, 1.0f};
            } else {
                color =  {1.0f, 0.0f, 0.0f, 1.0f};
            }
            Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , color, 256 );
        }
    }

    void Body::Move( const float speed ) {
        b2Vec2 vel = m_Body->GetLinearVelocity();
        float velChange = speed - vel.x;
        float impulse = m_Body->GetMass() * velChange;
        m_Body->ApplyLinearImpulse( b2Vec2(impulse,0), m_Body->GetWorldCenter(), true );
    }

    void Body::ApplyForce( const math::Vector2f& force ) {
        m_Body->ApplyLinearImpulse( b2Vec2(m_Body->GetMass()*force.m_X,m_Body->GetMass()*force.m_Y), m_Body->GetWorldCenter(), true );
    }

    void Body::AddBox( const math::Vector2f& position, 
                       const math::Vector2f extent, 
                       const BoxType boxType,
                       void* userData ) {
        b2PolygonShape box;
        box.SetAsBox(extent.m_X, extent.m_Y, b2Vec2(position.m_X, position.m_Y), 0.0f);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.03f;
        fixtureDef.userData = userData == NULL? this : userData;

        switch(boxType) {
            case E_SOLID:
                break;
            case E_SENSOR:
                fixtureDef.isSensor = true;
                break;
        }
        m_Fixtures.push_back(m_Body->CreateFixture(&fixtureDef));
    }
}
