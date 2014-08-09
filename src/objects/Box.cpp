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

#include "Box.hpp"
#include "Context.hpp"

namespace sacman {

    const char* Box::m_Type = "box";

    Box::Box( const char* name,
            const BodyType bodyType,
            const BehaviorType behavior,
            const math::Vector2f& position,
            const math::Vector2f& extent,
            void* userData ) :
        Entity( name ),
        m_BodyType( bodyType ),
        m_Behavior( behavior ),
        m_UserData( userData )
    {
        m_Position = position;
        m_Extent = extent;
    }

    Box::~Box() {
    }

    void Box::Enter( Level* level ) {
        b2BodyDef bodyDef;
        bodyDef.userData = m_UserData != NULL ? m_UserData : this;
        b2FixtureDef fixtureDef;
        b2PolygonShape box;
        switch(m_BodyType) {
            case E_DYNAMIC:
                bodyDef.type = b2_dynamicBody;
                bodyDef.fixedRotation = true;
                break;
            case E_STATIC:
                break;
        }

        bodyDef.position.Set( m_Position.m_X + 0.5f*m_Extent.m_X, m_Position.m_Y + 0.5f*m_Extent.m_Y);
        m_Body = level->B2World().CreateBody(&bodyDef);
        box.SetAsBox(0.5f*m_Extent.m_X, 0.5f*m_Extent.m_Y);
        fixtureDef.shape = &box;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.03f;

        switch(m_Behavior) {
            case E_SOLID:
                break;
            case E_SENSOR:
                fixtureDef.isSensor = true;
                break;
        }
        m_Fixture = m_Body->CreateFixture(&fixtureDef);
    }

    void Box::Leave( Level* level ) {
        m_Body->DestroyFixture( m_Fixture );
        level->B2World().DestroyBody(m_Body);
    }

    math::Vector2f Box::Position() const {
        b2Vec2 position = m_Body->GetPosition();
        const b2AABB& aabb = m_Fixture->GetAABB(0);
        b2Vec2 extents = aabb.GetExtents();
        return { position.x - extents.x, position.y-extents.y}; 
    }

    math::Vector2f Box::Extent() const {
        const b2AABB& aabb = m_Fixture->GetAABB(0);
        b2Vec2 extents = aabb.GetExtents();
        return {extents.x*2.0f, extents.y*2.0f};
    }

    const char* Box::Type() const {
        return m_Type;
    }

    void Box::SetPosition( const math::Vector2f& position ) {
        m_Body->SetTransform( b2Vec2(position.m_X, position.m_Y), 0.0f);
    }

    void Box::DrawShape( const double elapsedTime, const int depth ) const {
        const b2AABB& aabb = m_Fixture->GetAABB(0);
        b2Vec2 center = aabb.GetCenter();
        b2Vec2 extents = aabb.GetExtents();
        dali::RGBAColor color;
        if(m_Fixture->IsSensor()) {
            color =  {0.0f, 0.0f, 1.0f, 1.0f};
        } else {
            color =  {1.0f, 0.0f, 0.0f, 1.0f};
        }
        Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , color, 256 );
    }

    void Box::Move( const float speed ) {
        b2Vec2 vel = m_Body->GetLinearVelocity();
        float velChange = speed - vel.x;
        float impulse = m_Body->GetMass() * velChange;
        m_Body->ApplyLinearImpulse( b2Vec2(impulse,0), m_Body->GetWorldCenter(), true );
    }

    void Box::ApplyForce( const math::Vector2f force ) {
        //m_Body->ApplyForceToCenter( b2Vec2(m_Body->GetMass()*force.m_X, m_Body->GetMass()*force.m_Y), true );
        m_Body->ApplyLinearImpulse( b2Vec2(m_Body->GetMass()*force.m_X,m_Body->GetMass()*force.m_Y), m_Body->GetWorldCenter(), true );
    }
}
