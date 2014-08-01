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

    Level::Level() 
    {
        m_Velocity = {0.0,0.0};
        m_Position = {0.0, 0.0};
        m_Scale = {2.0, 2.0};
    }

    Level::~Level() {
    }

    void Level::StartUp() {
        m_Character = Context::m_ResourceLibrary.LoadSprite("prova");

    }

    void Level::ShutDown() {

    }

    void Level::ProcessEvents() {
        m_Velocity = {0.0f, 0.0f};
        SDL_Event event;
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            EventType eType = NONE;
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    eType = K_ESC;
                    Context::Exit();
                    break;
                case SDLK_UP:
            //        eType = K_UP;
                    m_Velocity.m_Y = 1.0f; 
                    break;
                case SDLK_DOWN:
             //       eType = K_DOWN;
                    m_Velocity.m_Y = -1.0f; 
                    break;
                case SDLK_LEFT:
              //      eType = K_LEFT;
                    m_Velocity.m_X = -1.0f; 
                    break;
                case SDLK_RIGHT:
               //     eType = K_RIGHT;
                    m_Velocity.m_X = 1.0f; 
                    break;
            }
            if( eType != NONE ) m_EventManager.LaunchEvent( eType, NULL );
        }
    }

    void Level::Draw( double elapsedTime ) {
        m_Position.m_X = m_Position.m_X + m_Velocity.m_X*elapsedTime;
        m_Position.m_Y = m_Position.m_Y + m_Velocity.m_Y*elapsedTime;
        m_Character->Draw( Context::m_Renderer, m_Position, m_Scale );
    }
}
