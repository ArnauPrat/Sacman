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

#include "StaticSprite.hpp"
#include "arnitech/system/Context.hpp"
#include "arnitech/renderer/Globals.hpp"


const char* atStaticSprite::m_Type = "staticsprite";

atStaticSprite::atStaticSprite(const char* name, const char* textureName, const atVector2f texCoords[4] ) : 
    atEntity(name) {
        m_Position = { 0.0f, 0.0f };
        m_Extent = { 0.0f, 0.0f };
        m_TexCoords.AddData(texCoords, 4);
        m_Texture = textureLoader.Load(textureName);
    }

atStaticSprite::~atStaticSprite() {

}

ATENTITY_POSITION_DEF(atStaticSprite) {
    return m_Position;
}

ATENTITY_EXTENT_DEF(atStaticSprite) {
    return m_Extent;
}

ATENTITY_SET_POSITION_DEF(atStaticSprite) {
    m_Position = position;
}

ATENTITY_TYPE_DEF(atStaticSprite) {
    return atStaticSprite::m_Type;
}

ATENTITY_DRAW_DEF(atStaticSprite) {
    atContext::m_Renderer.Draw(&m_TexCoords, m_Texture, depth, { m_Position.m_X - m_Extent.m_X, m_Position.m_Y - m_Extent.m_Y }, { m_Extent.m_X*2.0f, m_Extent.m_Y*2.0f });
}

ATENTITY_DRAW_SHAPE_DEF(atStaticSprite) {
    //        Context::m_Renderer.DrawBox({ m_Position.m_X - m_Extent.m_X, m_Position.m_Y - m_Extent.m_Y }, { m_Extent.m_X*2.0f, m_Extent.m_Y*2.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 256);
}

ATENTITY_ENTER_DEF(atStaticSprite) {
    m_Position = position;
    m_Extent = extent;
}

ATENTITY_LEAVE_DEF(atStaticSprite) {
}
