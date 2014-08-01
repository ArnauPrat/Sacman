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

#include "Context.hpp"
#include "Events.hpp"
#include "dali/Sprite.hpp"
#include <iostream>

namespace sacman {

    SDL_GLContext   Context::m_GLcontext;
    SDL_Window*     Context::m_Window = NULL;
    Config          Context::m_Config;
    bool            Context::m_Run;
    Level           Context::m_CurrentLevel;
    dali::ResourceLibrary Context::m_ResourceLibrary;
    dali::Renderer  Context::m_Renderer(Context::m_ResourceLibrary);

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
    }

    void Context::StartUp( const Config& config ) {
        m_Config = config;
        m_Run = true;
        SDLStartUp();
        m_Renderer.StartUp( m_Config.m_RendererConfig );
        m_CurrentLevel.StartUp();
    }

    void Context::ShutDown() {
        m_CurrentLevel.ShutDown();
        m_Renderer.ShutDown();
        SDLShutDown();
    }

    void Context::StartGameLoop() {
        while (m_Run) {
            m_CurrentLevel.ProcessEvents();
            m_Renderer.BeginFrame();
            m_CurrentLevel.Draw();
            m_Renderer.EndFrame();
            SDL_GL_SwapWindow(m_Window);
        }
    }

    void Context::Exit() {
        m_Run = false;
    }
}
