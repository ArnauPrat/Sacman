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

#include "arnitech/scene/Level.hpp"
#include "arnitech/scene/Entity.hpp"
#include "arnitech/math/Types.hpp"
#include <Box2D/Box2D.h>


enum BodyType{
    E_STATIC,
    E_DYNAMIC
};

enum BoxType {
    E_SOLID,
    E_SENSOR
};

class Body : public atEntity {
    public:
        Body( const char* name, 
                const BodyType type );

        ~Body();

        atVector2f Position() const ;
        atVector2f Extent() const ;
        void SetPosition( const atVector2f& position );
        const char* Type() const;
        void DrawShape( const double elapsedTime, const int depth ) const;
        void Enter( atLevel* level, const atVector2f position, const atVector2f& extent);
        void Leave( atLevel* level );
        void Move( const float speed );
        void ApplyForce( const atVector2f& force );
        void AddBox( const atVector2f& position, 
                const atVector2f extent, 
                const BoxType boxType,
                void* userData = NULL );


    private:
        static const char*                  m_Type;
        b2Body*                             m_Body;
        std::vector<b2Fixture*>             m_Fixtures;
        BodyType                            m_BodyType;
        atVector2f                          m_Position;
};

#endif

