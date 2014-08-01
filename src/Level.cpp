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
#include "Context.hpp"

namespace sacman {

    Level::Level() {
    }

    Level::~Level() {
    }

    void Level::StartUp() {
        m_Character = Context::m_ResourceLibrary.LoadSprite("prova");

    }

    void Level::ShutDown() {

    }

    void Level::ProcessEvents() {
        SDL_Event event;
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            /* If a quit event has been sent */
            EventType eType = NONE;
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    eType = K_ESC;
                    Context::Exit();
                    break;
                case SDLK_UP:
                    eType = K_UP;
                    break;
                case SDLK_DOWN:
                    eType = K_DOWN;
                    break;
                case SDLK_LEFT:
                    eType = K_LEFT;
                    break;
                case SDLK_RIGHT:
                    eType = K_RIGHT;
                    break;
            }
            if( eType != NONE ) m_EventManager.LaunchEvent( eType, NULL );
        }
    }

    void Level::Draw() {
        m_Character->Draw( Context::m_Renderer );
    }
}
