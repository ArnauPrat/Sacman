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

#include "Level.hpp"
#include "LoadLevel.hpp"
#include "arnitech/resources/Resources.hpp"
#include <cassert>
#include <cstring>

Level::Level( const char* name ) {
    assert(std::strlen(name) < SACMANLEVEL_FILE_NAME);
    std::strcpy(m_LevelName, name);
}

Level::~Level() {
}

ATLEVEL_START_UP_DEF(Level) {
    Load(m_LevelName,*this);
}

ATLEVEL_SHUT_DOWN_DEF(Level) {
}


