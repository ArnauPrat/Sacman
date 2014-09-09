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

#include "objects/StaticSprite.hpp"
#include "dali/Dali.hpp"
#include "Context.hpp"


namespace sacman {

    const char* StaticSprite::m_Type = "staticsprite";

    StaticSprite::StaticSprite(const char* name, const char* textureName, const math::Vector2f texCoords[4] ) : 
        Entity(name) {
        m_Position = { 0.0f, 0.0f };
        m_Extent = { 0.0f, 0.0f };
        m_TexCoords.AddData(texCoords, 4);
        m_Texture = dali::textureLoader.Load(textureName);
    }

    StaticSprite::~StaticSprite() {

    }

    math::Vector2f StaticSprite::Position() const {
        return m_Position;
    }

    math::Vector2f StaticSprite::Extent() const {
        return m_Extent;
    }

    void StaticSprite::SetPosition(const math::Vector2f& position) {
        m_Position = position;
    }

    const char* StaticSprite::Type() const {
        return StaticSprite::m_Type;
    }

    void StaticSprite::Draw(const double elapsedTime, const int depth) const {
        Context::m_Renderer.Draw(&m_TexCoords, m_Texture, depth, { m_Position.m_X - m_Extent.m_X, m_Position.m_Y - m_Extent.m_Y }, { m_Extent.m_X*2.0f, m_Extent.m_Y*2.0f });
    }

    void StaticSprite::DrawShape(const double elapsedTime, const int depth) const {
//        Context::m_Renderer.DrawBox({ m_Position.m_X - m_Extent.m_X, m_Position.m_Y - m_Extent.m_Y }, { m_Extent.m_X*2.0f, m_Extent.m_Y*2.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 256);
    }

    void StaticSprite::Enter(Level* level, const math::Vector2f position, const math::Vector2f& extent) {
        m_Position = position;
        m_Extent = extent;
    }

    void StaticSprite::Leave(Level* level) {
    }
}
