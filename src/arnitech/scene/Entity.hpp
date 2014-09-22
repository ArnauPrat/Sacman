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

#include "common/Platform.hpp"
#include "common/Macros.hpp"
#include "math/Types.hpp"
#include "physics/Collision.hpp"


#define ATENTITY_NAME_LENGTH 32

#define ATENTITY_CLASS(name)    class name : public atEntity
#define ATENTITY_DRAW           virtual void Draw( const double elapsedTime, const int depth ) const;
#define ATENTITY_DRAW_SHAPE     virtual void DrawShape( const double elapsedTime, const int depth ) const;
#define ATENTITY_UPDATE         virtual void Update( const double elapsedTime );
#define ATENTITY_COLLIDE        virtual void Collide( const atCollision& collision );
#define ATENTITY_ENTER          virtual void Enter( atLevel* level , const atVector2f position, const atVector2f& extent);
#define ATENTITY_LEAVE          virtual void Leave( atLevel* level );
#define ATENTITY_ABSTRACT       virtual atVector2f  Position() const; \
                                virtual void  SetPosition( const atVector2f& position ); \
                                virtual atVector2f  Extent() const; \
                                virtual const char* Type() const; 

#define ATENTITY_DRAW_DEF(className)          void className::Draw( const double elapsedTime, const int depth ) const
#define ATENTITY_DRAW_SHAPE_DEF(className)    void className::DrawShape( const double elapsedTime, const int depth ) const
#define ATENTITY_UPDATE_DEF(className)        void className::Update( const double elapsedTime )
#define ATENTITY_COLLIDE_DEF(className)       void className::Collide( const atCollision& collision )
#define ATENTITY_ENTER_DEF(className)         void className::Enter( atLevel* level , const atVector2f position, const atVector2f& extent)
#define ATENTITY_LEAVE_DEF(className)         void className::Leave( atLevel* level )

#define ATENTITY_POSITION_DEF(className)       atVector2f  className::Position() const 
#define ATENTITY_SET_POSITION_DEF(className)   void        className::SetPosition( const atVector2f& position ) 
#define ATENTITY_EXTENT_DEF(className)         atVector2f  className::Extent() const 
#define ATENTITY_TYPE_DEF(className)           const char* className::Type() const 



typedef short atEntityId;
class atLevel;

class atEntity {
    ATNON_COPYABLE(atEntity)
    static atEntityId m_CurrentId;
    public:
    ATEXPORT atEntity( const char* name );
    ATEXPORT virtual ~atEntity();

    ATEXPORT virtual void        Draw( const double elapsedTime, const int depth ) const;
    ATEXPORT virtual void        DrawShape( const double elapsedTime, const int depth ) const;
    ATEXPORT virtual void        Update( const double elapsedTime );
    ATEXPORT virtual void        Collide( const atCollision& collision );
    ATEXPORT virtual void        Enter( atLevel* level , const atVector2f position, const atVector2f& extent);
    ATEXPORT virtual void        Leave( atLevel* level );
    ATEXPORT virtual atVector2f  Position() const = 0;
    ATEXPORT virtual void        SetPosition( const atVector2f& position ) = 0;
    ATEXPORT virtual atVector2f  Extent() const = 0;
    ATEXPORT virtual const char* Type() const = 0;
    ATEXPORT const char*         Name() const;
    ATEXPORT atEntityId          Id() const;
    ATEXPORT int                 Depth() const;
    ATEXPORT void                SetDepth( int depth );

    protected:
    atEntityId          m_Id;
    char                m_Name[ATENTITY_NAME_LENGTH];
    int                 m_Depth;
};

#endif

