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
#include "Tiled.hpp"
#include "PathFinder.hpp"

namespace sacman {

    Level::Level()
    {
        m_Velocity = {0,0};
    }

    Level::~Level() {
    }

    void Level::StartUp() {
        m_Character.Load(PathFinder::FindDirectory("character.sprite"));
        TiledLevel* level = LoadTiledLevel(PathFinder::FindDirectory("test.json"));
        DeleteTiledLevel( level );
    }

    void Level::ShutDown() {
    }

    void Level::ProcessEvents() {
        m_Velocity = {0, 0};
        SDL_Event event;
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            EventType eType = NONE;
            if( event.key.type == SDL_KEYDOWN ) { 
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        eType = K_ESC;
                        Context::Exit();
                        break;
                    case SDLK_LEFT:
                        //      eType = K_LEFT;
                        m_Velocity.m_X = -1; 
                        if( !m_Character.IsAnimationRunning( "WalkLeft" ) ) {
                            m_Character.LaunchAnimation( "WalkLeft", 1.0f, false );
                        }
                        break;
                    case SDLK_RIGHT:
                        //     eType = K_RIGHT;
                        m_Velocity.m_X = 1; 
                        if( !m_Character.IsAnimationRunning( "WalkRight" ) ) {
                            m_Character.LaunchAnimation( "WalkRight", 1.0f, false );
                        }
                        break;
                }
            } else if(event.key.type == SDL_KEYUP) {
                m_Character.StopAnimation();
            }
            if( eType != NONE ) m_EventManager.LaunchEvent( eType, NULL );
        }

    }

    void Level::Draw( double elapsedTime ) {
        m_Character.m_Position.m_X = m_Character.m_Position.m_X + m_Velocity.m_X*elapsedTime;
        m_Character.m_Position.m_Y = m_Character.m_Position.m_Y + m_Velocity.m_Y*elapsedTime;
        m_Character.Draw( elapsedTime );
    }
}
