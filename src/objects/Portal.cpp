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

#include "Portal.hpp"
#include "dali/Globals.hpp"
#include <cstring>

namespace sacman {

    const char* Portal::m_Type = "portal";

    Portal::Portal( const char* name, 
                    const char* targetLevel,
                    const math::Vector2f& targetPosition,
                    const math::Vector2f& position, 
                    const math::Vector2f& extent ) :
        Entity( name ),
        m_Position( position ),
        m_Scale( extent ),
        m_Texture( NULL ),
        m_TargetPosition( targetPosition ),
        m_Box("",E_STATIC,E_SENSOR,position,extent, this )
    {
        if( targetLevel ) {
            assert(std::strlen(targetLevel) < LEVEL_NAME_LENGTH); 
            std::strcpy(m_TargetLevel, targetLevel);
        } else {
            std::strcpy(m_TargetLevel,"");
        }
    }

    Portal::~Portal() {
    }


    void Portal::Draw( const double elapsedTime, const int depth ) const {
        if(m_Texture) {
            Context::m_Renderer.Draw( &m_TexCoords, m_Texture, depth, m_Position, m_Scale );
        }
    }

    void Portal::DrawShape( const double elapsedTime, const int depth ) const {
        m_Box.DrawShape(elapsedTime,depth);
    }

    void Portal::Update( const double elapsedTime ) {
    }

    void Portal::Enter( Level* level ) {
        m_Box.Enter( level );
    }

    void Portal::Leave( Level* level ) {
        m_Box.Leave( level );
    }

    void Portal::LoadTexture( const char* textureName, const math::Vector2f texCoords[4] ) {
        m_Texture = dali::textureLoader.Load(textureName);
        m_TexCoords.AddData(texCoords,4);
    }

    math::Vector2f Portal::Position() const {
        return m_Box.Position();
    }

    void Portal::SetPosition( const math::Vector2f& position ) {
        m_Box.SetPosition(position);
    }

    math::Vector2f Portal::Extent() const {
        return m_Box.Extent();
    }

    const char* Portal::Type() const {
        return m_Type;
    }
}

