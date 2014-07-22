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

#include "Events.hpp"
#include "Context.hpp"
#include "dali/dali.hpp"
#include <iostream>

namespace sacman {

    SDL_GLContext   Context::m_GLcontext;
    SDL_Window*     Context::m_Window = NULL;
    Config          Context::m_Config;
    bool            Context::m_Run;
    dali::Renderer  Context::m_Renderer;
    EventManager    Context::m_EventManager;

    void Context::SDLStartUp() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            exit(1);
        }
        // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
        m_Window = SDL_CreateWindow(
                "SDL2/OpenGL Demo", 0, 0, 
                m_Config.m_RendererConfig.m_ViewportWidth,
                m_Config.m_RendererConfig.m_ViewportHeight, 
                SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

        // Create an OpenGL context associated with the window.
        m_GLcontext = SDL_GL_CreateContext(m_Window);
    }

    void Context::SDLShutDown() {
        SDL_GL_DeleteContext(m_GLcontext);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    void Context::ProcessEvents() {
        SDL_Event event;
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            /* If a quit event has been sent */
            EventType eType = NONE;
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    eType = K_ESC;
                    m_Run = false;
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

    void Context::StartUp( const Config& config ) {
        m_Config = config;
        m_Run = true;
        SDLStartUp();
        m_Renderer.StartUp( m_Config.m_RendererConfig );
    }

    void Context::ShutDown() {
        m_Renderer.ShutDown();
        SDLShutDown();
    }

    void Context::StartGameLoop() {
        dali::ResourceLoader<dali::Texture> textureLoader;
        dali::Texture* texture = textureLoader.Load( "./textures/ball.png");
        dali::Vector2fBuffer vertices;
        dali::Vector2fBuffer texCoords;
        dali::Vector2f data[4];
        data[0].m_X = 0.0f;
        data[0].m_Y = 0.0f;
        data[1].m_X = 1.0f;
        data[1].m_Y = 0.0f;
        data[2].m_X = 1.0f;
        data[2].m_Y = 1.0f;
        data[3].m_X = 0.0f;
        data[3].m_Y = 1.0f;
        vertices.AddData( data, 4 );
        texCoords.AddData( data, 4 );
        dali::IndexBuffer indices;
        unsigned short indexData[6];
        indexData[0] = 0;
        indexData[1] = 1;
        indexData[2] = 3;
        indexData[3] = 1;
        indexData[4] = 2;
        indexData[5] = 3;
        indices.AddData( indexData, 6 );
        while (m_Run) {
            ProcessEvents();
            m_Renderer.BeginFrame();
            m_Renderer.Draw( vertices, texCoords, indices, *texture, {2.0f, 2.0f} );
            m_Renderer.EndFrame();
            SDL_GL_SwapWindow(m_Window);
        }
    }
}
