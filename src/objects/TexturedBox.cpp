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

#include "TexturedBox.hpp"
#include "dali/Globals.hpp"
#include "Context.hpp"

namespace sacman {

    TexturedBox::TexturedBox( const char* name, 
                              Level& level, 
                              const math::Vector2f& position, 
                              const math::Vector2f& scale ) :
        Entity( name, level ),
        m_Position( position ), 
        m_Scale( scale ),
        m_Texture( NULL ),
        m_Body( NULL ),
        m_Fixture( NULL ) {
            std::cout << position.m_X << " " << position.m_Y << std::endl;
            std::cout << scale.m_X << " " << scale.m_Y << std::endl;

        }

    TexturedBox::~TexturedBox() {

        if( m_Fixture ) {
            m_Body->DestroyFixture( m_Fixture );
        }
        
        if( m_Body ) {
            m_Level.B2World().DestroyBody( m_Body );
        }
    }


    void TexturedBox::Draw( const double elapsedTime, const int depth ) const {
        if( m_Texture ) {
            Context::m_Renderer.Draw( m_TexCoords, *m_Texture, depth, m_Position, m_Scale );
        }

    }

    void TexturedBox::DrawShape( const double elapsedTime, const int depth ) const  {
        if( m_Fixture ) {
            const b2AABB& aabb = m_Fixture->GetAABB(0);
            b2Vec2 center = aabb.GetCenter();
            b2Vec2 extents = aabb.GetExtents();
            Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , {1.0f, 0.0f, 0.0f, 1.0f} );
        }
    }

    void TexturedBox::SetBoxShape() {
        b2BodyDef bodyDef;
        bodyDef.position.Set(m_Position.m_X+0.5*m_Scale.m_X,m_Position.m_Y+0.5*m_Scale.m_Y);
        m_Body = m_Level.B2World().CreateBody(&bodyDef);
        b2PolygonShape box;
        box.SetAsBox(0.5*m_Scale.m_X, 0.5*m_Scale.m_Y);
        m_Fixture = m_Body->CreateFixture(&box, 0.0f);
    }

    void TexturedBox::LoadTexture( const char* textureName, const math::Vector2f texCoords[4] ) {
        m_TexCoords.AddData( texCoords, 4 );
        m_Texture = dali::textureLoader.Load( textureName );
    }

    math::Vector2f TexturedBox::Position() const {
        return m_Position;
    }

    void TexturedBox::SetPosition( const math::Vector2f& position ) {
        assert(false);
    }

    math::Vector2f TexturedBox::Extent() const {
        return m_Scale;
    }

    void TexturedBox::SetExtent( const math::Vector2f& extent ) {
        assert(false);
    }
}
