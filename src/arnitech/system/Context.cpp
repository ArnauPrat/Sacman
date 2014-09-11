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
#include "renderer/Sprite.hpp"
#include <iostream>

/** PROFILING STUFF **/
#if defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#endif

#define MICRO_PROFILE_IMPL
#define MICROPROFILE_UI 0
#define MICROPROFILE_GPU_TIMERS 0
#define MICROPROFILE_WEBSERVER 0 
#include <microprofile.h>
MICROPROFILE_DEFINE(MAIN, "MAIN", "Main", 0xff0000);
/*********************/

#define FRAMES_PER_SECOND 60


    SDL_GLContext       atContext::m_GLcontext;
    SDL_Window*         atContext::m_Window = NULL;
    atConfig              atContext::m_Config;
    bool                atContext::m_Run;
    atLevel               atContext::m_CurrentLevel;
    atRenderer      atContext::m_Renderer;
    atLog*       atContext::log;

    void atContext::SDLStartUp() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
            log->Error("SDL_Init Error: ", SDL_GetError());
            exit(1);
        }
        // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
        m_Window = SDL_CreateWindow( "Sacman", 0, 0, 
                                      m_Config.m_RendererConfig.m_ViewportWidth,
                                      m_Config.m_RendererConfig.m_ViewportHeight, 
                                      SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

        // Create an OpenGL context associated with the window.
        m_GLcontext = SDL_GL_CreateContext(m_Window);
    }

    void atContext::SDLShutDown() {
        SDL_GL_DeleteContext(m_GLcontext);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    void atContext::ProcessEvents() {
    }

    void atContext::StartUp( const char* configFileName ) {
        log = new atLog("sacman.log");
        Load(m_Config, configFileName);
        m_Run = true;
        SDLStartUp();
        m_Renderer.StartUp( m_Config.m_RendererConfig );
        m_CurrentLevel.StartUp();
    }

    void atContext::ShutDown() {
        m_CurrentLevel.ShutDown();
        m_Renderer.ShutDown();
        SDLShutDown();
        delete log;
    }

    void atContext::StartGameLoop() {
        MicroProfileOnThreadCreate("Main");
        MicroProfileSetForceEnable(true);
        MicroProfileSetEnableAllGroups(true);
        MicroProfileSetForceMetaCounters(true);
        unsigned int currentTime = SDL_GetTicks();
        while (m_Run) {
            unsigned int newTime = SDL_GetTicks();
            double elapsedTime = (newTime - currentTime);
            MICROPROFILE_SCOPE(MAIN);
            {
                currentTime += static_cast<unsigned int>(elapsedTime);
                MICROPROFILE_SCOPEI("Main", "Dummy", 0xff3399ff);
                MICROPROFILE_META_CPU("Frame", 1);
                m_CurrentLevel.Update(elapsedTime / 1000.0f);
                m_Renderer.BeginFrame();
                m_CurrentLevel.Draw(elapsedTime / 1000.0);
                if (m_Config.m_DrawDebug)
                    m_CurrentLevel.DrawDebug(elapsedTime / 1000.0);
            }
            MicroProfileFlip();
            m_Renderer.EndFrame();
            SDL_GL_SwapWindow(m_Window);
        }
        MicroProfileShutdown();
    }

    void atContext::Exit() {
        m_Run = false;
    }

    atConfig atContext::GetConfig() {
        return m_Config;
    }
