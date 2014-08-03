/*Dali is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Dali is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

namespace dali {
    class PathFinder {
        public:
            PathFinder();
            ~PathFinder();
            void AddDirectory( const char* directory );
            const char* FindPath( const char* fileName );

        private:
            char                     m_Buffer[2048];
            std::vector<const char*> m_Directories;
    };
}

#endif


