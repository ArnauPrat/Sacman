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

#ifndef ATENTITY_H
#define ATENTITY_H

#include "math/Types.hpp"
#include "physics/Collision.hpp"


#define ATENTITY_NAME_LENGTH 32
typedef short atEntityId;
class atLevel;

class atEntity {
    static atEntityId m_CurrentId;
    public:
    atEntity( const char* name );
    virtual ~atEntity();

    virtual void        Draw( const double elapsedTime, const int depth ) const;
    virtual void        DrawShape( const double elapsedTime, const int depth ) const;
    virtual void        Update( const double elapsedTime );
    virtual void        Collide( const atCollision& collision );
    virtual void        Enter( atLevel* level , const atVector2f position, const atVector2f& extent);
    virtual void        Leave( atLevel* level );
    virtual atVector2f  Position() const = 0;
    virtual void        SetPosition( const atVector2f& position ) = 0;
    virtual atVector2f  Extent() const = 0;
    virtual const char* Type() const = 0;
    const char*         Name() const;
    atEntityId          Id() const;
    int                 Depth() const;
    void                SetDepth( int depth );

    protected:
    atEntityId          m_Id;
    char                m_Name[ATENTITY_NAME_LENGTH];
    int                 m_Depth;
};

#endif

