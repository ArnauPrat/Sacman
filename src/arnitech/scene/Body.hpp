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

#ifndef SACMANBODY_H
#define SACMANBODY_H

#include "common/Platform.hpp"
#include "common/Macros.hpp"
#include "scene/Level.hpp"
#include "scene/Entity.hpp"
#include "math/Types.hpp"
#include <Box2D/Box2D.h>


enum atBodyType{
    E_STATIC,
    E_DYNAMIC
};

enum atBoxType {
    E_SOLID,
    E_SENSOR
};

ATENTITY_CLASS(atBody) {
    ATNON_COPYABLE(atBody)
    public:
        ATEXPORT atBody( const char* name, 
                const atBodyType type );
        ATEXPORT ~atBody();

        ATENTITY_DRAW_SHAPE;
        ATENTITY_ENTER;
        ATENTITY_LEAVE;
        ATENTITY_ABSTRACT;
        
        ATEXPORT void Move( const float speed );
        ATEXPORT void ApplyForce( const atVector2f& force );
        ATEXPORT void AddBox( const atVector2f& position, 
                const atVector2f extent, 
                const atBoxType boxType,
                void* userData = NULL );

    private:
        static const char*                  m_Type;
        b2Body*                             m_Body;
        std::vector<b2Fixture*>             m_Fixtures;
        atBodyType                          m_BodyType;
        atVector2f                          m_Position;
};

#endif

