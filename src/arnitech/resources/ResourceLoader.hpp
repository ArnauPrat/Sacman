/*Dali is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Dali is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ATRESOURCELOADER_H
#define ATRESOURCELOADER_H

#include "common/Platform.hpp"
#include "system/Context.hpp"
#include "common/Macros.hpp"
#include "PathFinder.hpp"
#include <cassert>
#include <iostream>
#include <map>

ATEXPORT extern atPathFinder pathFinder;

template <class T> class atResourceLoader {
    ATNON_COPYABLE(atResourceLoader)
    public:
        atResourceLoader() {
        }

        ~atResourceLoader() {
            for( auto it = m_Resources.begin(); it != m_Resources.end(); ++it ) {
                delete (*it).second;
            }
        }

        /** @brief Loads a resource.
         *  @param filename The path to the resource.
         *  @return A pointer to the resource.*/
        T* Load( const char* filename ) {
            assert(filename != NULL);
            auto it = m_Resources.find( filename ); 
            if( it != m_Resources.end() ) {
                return (*it).second;
            }
            T* resource = new T();
            const char* path = pathFinder.FindPath(filename);
            resource->Load( path );
            m_Resources.insert( std::pair< std::string, T*>( filename, resource ) );
            atContext::log->Print("Loaded resource: %s", path); 
            return resource;
        }

    private:
        std::map<std::string,T*>    m_Resources; /**< @brief A map between filenames and resources.*/
};

#endif
