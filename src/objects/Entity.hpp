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
#include "Collision.hpp"

namespace sacman {

#define ENTITY_NAME_LENGTH 32
    typedef short EntityId;
    class Level;

    class Entity {
        static EntityId m_CurrentId;
        public:
            Entity( const char* name );
            virtual ~Entity();

            virtual void Draw( const double elapsedTime, const int depth ) const;
            virtual void DrawShape( const double elapsedTime, const int depth ) const;
            virtual void Update( const double elapsedTime );
            virtual void Collide( const Collision& collision );
            virtual void Enter( Level* level );
            virtual void Leave( Level* level );
            virtual math::Vector2f Position() const = 0;
            virtual void SetPosition( const math::Vector2f& position ) = 0;
            virtual math::Vector2f Extent() const = 0;
            virtual const char* Type() const = 0;
            const char* Name() const;
            EntityId    Id() const;
            int         Depth() const;
            void        SetDepth( int depth );

        protected:
            EntityId    m_Id;
            char        m_Name[ENTITY_NAME_LENGTH];
            int         m_Depth;
    };
}

#endif

