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

#ifndef SACMANLEVEL_HPP
#define SACMANLEVEL_HPP

#include "arnitech/scene/Level.hpp"

#define SACMANLEVEL_FILE_NAME_LENGTH 32

ATLEVEL_CLASS(Level) {
    public:
        Level( const char* name );
        ~Level( );
        ATLEVEL_START_UP
        ATLEVEL_SHUT_DOWN

    private:
        char    m_LevelName[SACMANLEVEL_FILE_NAME_LENGTH];
};


#endif
