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

#include "Object.hpp"
#include "dali/Globals.hpp"
#include "Context.hpp"

namespace sacman {

    Object::Object( Level& level, const math::Vector2f& position, const math::Vector2f& scale ) :
        m_Level( level ),
        m_Position( position ), 
        m_Scale( scale ),
        m_Texture( NULL ),
        m_Body( NULL ),
        m_Fixture( NULL ) {

        }

    Object::~Object() {

        if( m_Fixture ) {
            m_Body->DestroyFixture( m_Fixture );
        }
        
        if( m_Body ) {
            m_Level.B2World().DestroyBody( m_Body );
        }
    }

    void Object::LoadTexture( const char* textureName, const math::Vector2f texCoords[4] ) {
        m_TexCoords.AddData( texCoords, 4 );
        m_Texture = dali::textureLoader.Load( textureName );
    }

    void Object::Draw( const int depth ) const {
        if( m_Texture ) {
            Context::m_Renderer.Draw( m_TexCoords, *m_Texture, depth, m_Position, m_Scale );
        }

    }

    void Object::DrawShape() const  {
        if( m_Fixture ) {
            const b2AABB& aabb = m_Fixture->GetAABB(0);
            b2Vec2 center = aabb.GetCenter();
            b2Vec2 extents = aabb.GetExtents();
            Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , {1.0f, 0.0f, 0.0f, 1.0f} );
        }
    }

    void Object::SetBoxShape() {
        b2BodyDef bodyDef;
        bodyDef.position.Set(m_Position.m_X+0.5*m_Scale.m_X,m_Position.m_Y+0.5*m_Scale.m_Y);
        m_Body = m_Level.B2World().CreateBody(&bodyDef);
        b2PolygonShape box;
        box.SetAsBox(0.5*m_Scale.m_X, 0.5*m_Scale.m_Y);
        m_Fixture = m_Body->CreateFixture(&box, 0.0f);
    }

    void Object::SetPolyShape( const math::Vector2f vertexes[], const int numVertexes ) {
    }
}
