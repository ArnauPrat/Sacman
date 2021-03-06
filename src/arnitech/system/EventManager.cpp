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

atEventManager::atEventManager() {
}

atEventManager::~atEventManager() {
}

void atEventManager::RegisterListener( atEventType eventType, std::function<void( std::shared_ptr<void>)> listener ) {
    m_EventTypeListeners[eventType].push_back(listener);
}

void atEventManager::RegisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener ) {
    m_StringListeners[eventType].push_back(listener);
}

void atEventManager::UnregisterListener( atEventType eventType, std::function<void( std::shared_ptr<void>)> listener ) {
    auto it = m_EventTypeListeners.find(eventType);
    if( it != m_EventTypeListeners.end() ) {
        std::list<std::function<void(std::shared_ptr<void>)> >& listeners = (*it).second;
        for( auto it2 = listeners.begin(); it2 != listeners.end(); ++it2 ) {
            if( (*it2).target< void(std::shared_ptr<void>)>() == 
                    listener.target< void(std::shared_ptr<void>) >() ) {
                (*it).second.erase( it2 );
                return;
            }
        } 
    }
}

void atEventManager::UnregisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener ) {
    auto it = m_StringListeners.find(eventType);
    if( it != m_StringListeners.end() ) {
        std::list<std::function<void(std::shared_ptr<void>)> >& listeners = (*it).second;
        for( auto it2 = listeners.begin(); it2 != listeners.end(); ++it2 ) {
            if( (*it2).target< void(std::shared_ptr<void>)>() == 
                    listener.target< void(std::shared_ptr<void>) >() ) {
                (*it).second.erase( it2 );
                return;
            }
        } 
    }
}

void atEventManager::LaunchEvent( atEventType eventType, std::shared_ptr<void> data ) {
    auto it = m_EventTypeListeners.find(eventType);
    if( it != m_EventTypeListeners.end() ) {
        std::list<std::function<void(std::shared_ptr<void>)> >& listeners = (*it).second;
        for( auto listener : listeners ) {
            listener(data);
        }
    }
}

void atEventManager::LaunchEvent( const char* eventType, std::shared_ptr<void> data ) {
    auto it = m_StringListeners.find(eventType);
    if( it != m_StringListeners.end() ) {
        std::list<std::function<void(std::shared_ptr<void>)> >& listeners = (*it).second;
        for( auto listener : listeners ) {
            listener(data);
        }
    }
}
