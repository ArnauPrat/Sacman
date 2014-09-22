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

#ifndef ATCAMERA_HPP
#define ATCAMERA_HPP

#include "common/Platform.hpp"
#include "common/Macros.hpp"
#include "Entity.hpp"

ATENTITY_CLASS(atCamera) {
    ATNON_COPYABLE(atCamera)
    public:
        ATEXPORT atCamera( const char* name );
        ATEXPORT virtual ~atCamera();

        ATENTITY_UPDATE
        ATENTITY_ENTER
        ATENTITY_LEAVE
        ATENTITY_ABSTRACT

        ATEXPORT void SetTarget( const char* name );
        ATEXPORT void SetLimitMin( const atVector2f& min );
        ATEXPORT void SetLimitMax( const atVector2f& max );

    private:
        static const char*  m_Type;
        char                m_Target[ATENTITY_NAME_LENGTH];
        atVector2f          m_Position;
        atVector2f          m_Extent;
        atLevel*            m_Level;
        atVector2f          m_LimitMin;
        atVector2f          m_LimitMax;
};

#endif
