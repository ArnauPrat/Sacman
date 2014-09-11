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


    atEntityId atEntity::m_CurrentId = 0;

    atEntity::atEntity( const char* name ) :
        m_Depth(256)
     {
        assert(std::strlen(name) < ENTITY_NAME_LENGTH);
        std::strcpy( m_Name, name );
        m_Id = m_CurrentId++;
    }

    atEntity::~atEntity() {
    }

    void atEntity::Draw( const double elapsedTime, const int depth ) const {
    }

    void atEntity::DrawShape( const double elapsedTime, const int depth ) const {
    }

    void atEntity::Update( const double elapsedTime ) {
    }

    void atEntity::Collide( const atCollision& collision ) {
    }

    void atEntity::Enter( atLevel* level, const atVector2f position, const atVector2f& extent) {
    }

    void atEntity::Leave( atLevel* level ) {
    }

    const char* atEntity::Name() const {
        return m_Name;
    }

    atEntityId atEntity::Id() const {
        return m_Id;
    }

    int         atEntity::Depth() const {
        return m_Depth;
    }

    void        atEntity::SetDepth( int depth ) {
        m_Depth = depth;
    }

