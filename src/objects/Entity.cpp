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

#include "Entity.hpp"
#include <cstring>
#include <cassert>

namespace sacman {

    EntityId Entity::m_CurrentId = 0;
    Entity::Entity( const char* name, Level& level ) :
        m_Level( level )
     {
        assert(std::strlen(name) < ENTITY_NAME_LENGTH);
        std::strcpy( m_Name, name );
        m_Id = m_CurrentId++;
    }

    Entity::~Entity() {
    }

    void Entity::Draw( const double elapsedTime, const int depth ) const {
    }

    void Entity::DrawShape( const double elapsedTime, const int depth ) const {
    }

    const char* Entity::Name() const {
        return m_Name;
    }

    EntityId Entity::Id() const {
        return m_Id;
    }
}

