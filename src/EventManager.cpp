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

#include "EventManager.hpp"

namespace sacman {

    void EventManager::RegisterListener( EventType eventType, void (*listener)(void*)) {
        m_EventTypeListeners[eventType].push_back(listener);
    }

    void EventManager::RegisterListener( const std::string& eventType, void (*listener)(void*)) {
        m_StringListeners[eventType].push_back(listener);
    }

    void EventManager::UnregisterListener( EventType eventType, void (*listener)(void*)) {
        auto it = m_EventTypeListeners.find(eventType);
        if( it != m_EventTypeListeners.end() ) {
           std::list< void (*)(void*) >& listeners = (*it).second;
           for( auto it2 = listeners.begin(); it2 != listeners.end(); ++it2 ) {
               if( (*it2) == listener ) {
                    (*it).second.erase( it2 );
                    return;
               }
           } 
        }
    }

    void EventManager::UnregisterListener( const std::string& eventType, void (*listener)(void*)) {
        auto it = m_StringListeners.find(eventType);
        if( it != m_StringListeners.end() ) {
           std::list< void (*)(void*) >& listeners = (*it).second;
           for( auto it2 = listeners.begin(); it2 != listeners.end(); ++it2 ) {
               if( (*it2) == listener ) {
                    (*it).second.erase( it2 );
                    return;
               }
           } 
        }
    }

    void EventManager::LaunchEvent( EventType eventType, void* data ) {
        auto it = m_EventTypeListeners.find(eventType);
        if( it != m_EventTypeListeners.end() ) {
           std::list< void (*)(void*) >& listeners = (*it).second;
            for( auto listener : listeners ) {
                listener(data);
            }
        }
    }

    void EventManager::LaunchEvent( const std::string& eventType, void* data ) {
        auto it = m_StringListeners.find(eventType);
        if( it != m_StringListeners.end() ) {
           std::list< void (*)(void*) >& listeners = (*it).second;
            for( auto listener : listeners ) {
                listener(data);
            }
        }
    }
}
