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

#include "PathFinder.hpp"
#include "common/Log.hpp"
#include "system/Context.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

atPathFinder::atPathFinder(){
}

atPathFinder::~atPathFinder() {
}

void atPathFinder::AddDirectory( const char* directory ) {
    char* dir = new char[ std::strlen(directory) + 1];
    std::strcpy( dir, directory );
    m_Directories.push_back( dir );
}

const char* atPathFinder::FindPath( const char* fileName ) {
    for( unsigned int i = 0; i < m_Directories.size(); ++i ) {
        std::sprintf( m_Buffer, "%s/%s",m_Directories[i], fileName );
        std::ifstream ifile( m_Buffer );
        if (ifile) { 
            return m_Buffer; 
        }
    } 
    atContext::log->Error("FILE %s not found", fileName);
    assert(false);
    return NULL;
}

