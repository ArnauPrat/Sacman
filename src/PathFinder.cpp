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

#include "PathFinder.hpp"
#include <cstring>
#include <fstream>
#include <cassert>
#include <iostream>

namespace sacman {

    std::vector<const char*> PathFinder::m_Directories;

    void PathFinder::AddDirectory( const char* directory ) {
        char* dir = new char[ std::strlen(directory) + 1];
        std::strcpy( dir, directory );
        m_Directories.push_back( dir );
    }

    const char* PathFinder::FindDirectory( const char* fileName ) {
        for( unsigned int i = 0; i < m_Directories.size(); ++i ) {
            char path[strlen(m_Directories[i]) + strlen(fileName) + 1];
            std::sprintf( path, "%s/%s",m_Directories[i], fileName );
            std::ifstream ifile( path );
            if (ifile) return m_Directories[i]; 
        } 
        std::cout << "FILE " << fileName << " not found" << std::endl;
        assert(false);
        return NULL;
    }


}

