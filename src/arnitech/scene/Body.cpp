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
#include "arnitech/system/Context.hpp"
#include "arnitech/renderer/Renderer.hpp"
#include "arnitech/renderer/Types.hpp"

#include <cfloat>


const char* atBody::m_Type = "body";

atBody::atBody( const char* name,
        const atBodyType bodyType) :
    atEntity( name ),
    m_BodyType( bodyType )
{
}

atBody::~atBody() {
}

ATENTITY_ENTER_DEF(atBody) {
    b2BodyDef bodyDef;
    switch(m_BodyType) {
        case E_DYNAMIC:
            bodyDef.type = b2_dynamicBody;
            bodyDef.fixedRotation = true;
            break;
        case E_STATIC:
            break;
    }

    bodyDef.position.Set( position.m_X, position.m_Y );
    m_Body = level->B2World().CreateBody(&bodyDef);
}

ATENTITY_LEAVE_DEF(atBody) {
    for( b2Fixture* fixture : m_Fixtures ) {
        m_Body->DestroyFixture( fixture );
    }
    level->B2World().DestroyBody(m_Body);
}

ATENTITY_POSITION_DEF(atBody) {
    b2Vec2 position = m_Body->GetPosition();
    return { position.x, position.y}; 
}

ATENTITY_EXTENT_DEF(atBody) {
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX); 
    for( b2Fixture* fixture : m_Fixtures ) {
        aabb.Combine(fixture->GetAABB(0));
    }
    b2Vec2 extents = aabb.GetExtents();
    return {extents.x, extents.y};
}

ATENTITY_TYPE_DEF(atBody) {
    return m_Type;
}

ATENTITY_SET_POSITION_DEF(atBody) {
    m_Body->SetTransform( b2Vec2(position.m_X, position.m_Y), 0.0f);
}

ATENTITY_DRAW_SHAPE_DEF(atBody) {
    for( b2Fixture* fixture : m_Fixtures ) {
        const b2AABB& aabb = fixture->GetAABB(0);
        b2Vec2 center = aabb.GetCenter();
        b2Vec2 extents = aabb.GetExtents();
        atRGBAColor color;
        if(fixture->IsSensor()) {
            color =  {0.0f, 0.0f, 1.0f, 1.0f};
        } else {
            color =  {1.0f, 0.0f, 0.0f, 1.0f};
        }
        atContext::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , color, 256 );
    }
}

void atBody::Move( const float speed ) {
    b2Vec2 vel = m_Body->GetLinearVelocity();
    float velChange = speed - vel.x;
    float impulse = m_Body->GetMass() * velChange;
    m_Body->ApplyLinearImpulse( b2Vec2(impulse,0), m_Body->GetWorldCenter(), true );
}

void atBody::ApplyForce( const atVector2f& force ) {
    m_Body->ApplyLinearImpulse( b2Vec2(m_Body->GetMass()*force.m_X,m_Body->GetMass()*force.m_Y), m_Body->GetWorldCenter(), true );
}

void atBody::AddBox( const atVector2f& position, 
                   const atVector2f extent, 
                   const atBoxType boxType,
                   void* userData ) {

    b2PolygonShape box;
    box.SetAsBox(extent.m_X, extent.m_Y, b2Vec2(position.m_X, position.m_Y), 0.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.03f;
    fixtureDef.restitution = 0.0f;
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
