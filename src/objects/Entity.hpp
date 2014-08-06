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

#ifndef SACMAN_ENTITY_H
#define SACMAN_ENTITY_H

#include "math/Types.hpp"

namespace sacman {

#define ENTITY_NAME_LENGTH 32
    typedef short EntityId;
    class Level;

    class Entity {
        static EntityId m_CurrentId;
        public:
            Entity( const char* name, Level& level );
            virtual ~Entity();

            virtual void Draw( const double elapsedTime, const int depth ) const;
            virtual void DrawShape( const double elapsedTime, const int depth ) const;
            virtual math::Vector2f Position() const = 0;
            virtual void SetPosition( const math::Vector2f& position ) = 0;
            virtual math::Vector2f Extent() const = 0;
            virtual void SetExtent( const math::Vector2f& extent ) = 0;
            const char* Name() const;
            EntityId    Id() const;

        protected:
            Level&      m_Level;

        private:
            EntityId    m_Id;
            char        m_Name[ENTITY_NAME_LENGTH];
    };
}

#endif

