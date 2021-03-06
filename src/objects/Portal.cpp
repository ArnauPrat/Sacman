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
#include "arnitech/resources/Resources.hpp"
#include <cstring>


const char* Portal::m_Type = "portal";

Portal::Portal( const char* name, 
        const char* targetLevel,
        const atVector2f& targetPosition) :
    atEntity( name ),
    m_Texture( NULL ),
    m_TargetPosition( targetPosition ),
    m_Body("",E_STATIC )
{
    if( targetLevel ) {
        assert(std::strlen(targetLevel) < ATLEVEL_NAME_LENGTH); 
        std::strcpy(m_TargetLevel, targetLevel);
    } else {
        std::strcpy(m_TargetLevel,"");
    }
}

Portal::~Portal() {
}


void Portal::Draw( const double elapsedTime, const int depth ) const {
    if(m_Texture) {
        atVector2f position = m_Body.Position();
        atVector2f extent = m_Body.Extent();
        position.m_X -= extent.m_X;
        position.m_Y -= extent.m_Y;
        extent.m_X *=2.0f;
        extent.m_Y *=2.0f;
        atContext::m_Renderer.Draw( &m_TexCoords, m_Texture, depth, position, extent );
    }
}

void Portal::DrawShape( const double elapsedTime, const int depth ) const {
    m_Body.DrawShape(elapsedTime,depth);
}

void Portal::Update( const double elapsedTime ) {
}

void Portal::Enter( atLevel* level, const atVector2f position, const atVector2f& extent) {
    m_Body.Enter( level, position, extent );
    m_Body.AddBox({0.0f, 0.0f}, extent, E_SENSOR);
}

void Portal::Leave( atLevel* level ) {
    m_Body.Leave( level );
}

void Portal::LoadTexture( const char* textureName, const atVector2f texCoords[4] ) {
    m_Texture = textureLoader.Load(textureName);
    m_TexCoords.AddData(texCoords,4);
}

atVector2f Portal::Position() const {
    return m_Body.Position();
}

void Portal::SetPosition( const atVector2f& position ) {
    m_Body.SetPosition(position);
}

atVector2f Portal::Extent() const {
    return m_Body.Extent();
}

const char* Portal::Type() const {
    return m_Type;
}
