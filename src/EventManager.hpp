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

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "Events.hpp"
#include <map>
#include <string>
#include <list>
#include <memory>
#include <functional>

namespace sacman {
    class EventManager {
        public:
            EventManager();
            ~EventManager();
            
            /** @brief Registers an event to listen to an event type.
             *  @param eventType the event type.
             *  @param listener the listener of the event.*/ 
            void RegisterListener( EventType eventType, std::function<void( std::shared_ptr<void>)> listener ); 

            /** @brief Registers an event to listen to an event type.
             *  @param eventType the event type.
             *  @param listener the listener of the event.*/ 
            void RegisterListener( const char* eventType,std::function<void( std::shared_ptr<void>)> listener  ); 

            /** @brief Unregisters an event to listen to an event type.
             *  @param eventType the event type.
             *  @param listener the listener of the event.*/ 
            void UnregisterListener( EventType eventType, std::function<void( std::shared_ptr<void>)> listener ); 

            /** @brief Unregisters an event to listen to an event type.
             *  @param eventType the event type.
             *  @param listener the listener of the event.*/ 
            void UnregisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener  ); 

            /** @brief Launchs an event.
             *  @param eventType the event type.
             *  @param data data associated with the event.*/
            void LaunchEvent( EventType eventType, std::shared_ptr<void> data );

            /** @brief Launchs an event.
             *  @param eventType the event type.
             *  @param data data associated with the event.*/
            void LaunchEvent( const char* eventType, std::shared_ptr<void> data );

        private:
            EventManager( const EventManager& eventManager );
            std::map<EventType, std::list<std::function<void( std::shared_ptr<void>)> > >     m_EventTypeListeners; 
            std::map<std::string, std::list<std::function<void( std::shared_ptr<void>)> > >   m_StringListeners; 
    };
}
#endif
