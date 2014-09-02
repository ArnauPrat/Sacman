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

#include "Occluder.hpp"


namespace sacman {

    const char* Occluder::m_Type = "occluder";

    Occluder::Occluder(const char* name, const char* textureName, const math::Vector2f texCoords[4]) : 
        Entity(name),
        m_Body( "", E_STATIC),
        m_Sprite("",textureName, texCoords){
    }

    Occluder::~Occluder() {

    }

    math::Vector2f Occluder::Position() const {
        return m_Body.Position();
    }

    math::Vector2f Occluder::Extent() const {
        return m_Body.Extent();
    }

    void Occluder::SetPosition(const math::Vector2f& position) {
        m_Body.SetPosition(position);
    }

    const char* Occluder::Type() const {
        return Occluder::m_Type;
    }

    void Occluder::Draw(const double elapsedTime, const int depth) const {
        m_Sprite.Draw(elapsedTime, depth);
    }

    void Occluder::DrawShape(const double elapsedTime, const int depth) const {
        m_Body.DrawShape(elapsedTime, depth);
    }

    void Occluder::Enter(Level* level, const math::Vector2f position, const math::Vector2f& extent) {
        m_Sprite.Enter(level, position, extent);
        m_Body.Enter( level, position, extent );
        m_Body.AddBox({ 0.0f, 0.0f }, extent, E_SENSOR, this);
    }

    void Occluder::Leave(Level* level) {
        m_Sprite.Leave(level);
        m_Body.Leave( level );
    }
}
