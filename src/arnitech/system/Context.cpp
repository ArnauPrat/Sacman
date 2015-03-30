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

#define FRAMES_PER_SECOND 60.0

SDL_GLContext       atContext::m_GLcontext;
SDL_Window*         atContext::m_Window = NULL;
atConfig            atContext::m_Config;
bool                atContext::m_Run;
atLevel*            atContext::m_CurrentLevel;
atRenderer          atContext::m_Renderer;
atLog*              atContext::log;

void atContext::SDLStartUp() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        log->Error("SDL_Init Error: ", SDL_GetError());
        exit(1);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    m_Window = SDL_CreateWindow( "Sacman", 0, 0, 
            m_Config.m_RendererConfig.m_ViewportWidth,
            m_Config.m_RendererConfig.m_ViewportHeight, 
            SDL_WINDOW_OPENGL);

    // Create an OpenGL context associated with the window.
    m_GLcontext = SDL_GL_CreateContext(m_Window);
    log->Print("OpenGL Version string %s",reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    int glslMajorVersion = -1, glslMinorVersion = -1;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glslMajorVersion);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glslMinorVersion);

    log->Print("Executing with with shader version %d.%d",glslMajorVersion, glslMinorVersion);
    SDL_GL_SetSwapInterval(1);
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
    Initialize(m_Config);
    Load(m_Config, configFileName);
    m_Run = true;
    SDLStartUp();
    m_Renderer.StartUp( m_Config.m_RendererConfig );
}

void atContext::PushLevel( atLevel* level ) {
    m_CurrentLevel = level;
    m_CurrentLevel->_StartUp();
}

void atContext::PopLevel( ) {
    m_CurrentLevel->_ShutDown();
    m_CurrentLevel = NULL;
}

void atContext::ShutDown() {
    m_Renderer.ShutDown();
    SDLShutDown();
    delete log;
}

void atContext::StartGameLoop() {
    unsigned int currentTime = SDL_GetTicks();
    double renderElapsed = 0.0;
    while (m_Run) {
        unsigned int newTime = SDL_GetTicks();
        double elapsedTime = (newTime - currentTime);
        currentTime += static_cast<unsigned int>(elapsedTime);
        m_CurrentLevel->SimulatePhysics(elapsedTime / 1000.0f);

        renderElapsed += elapsedTime;
        while (renderElapsed >= 1000/FRAMES_PER_SECOND) {
            m_CurrentLevel->Update(renderElapsed / 1000.0f);
            m_Renderer.BeginFrame();
            m_CurrentLevel->Draw(renderElapsed / 1000.0);
            if (m_Config.m_DrawDebug)
                m_CurrentLevel->DrawDebug(renderElapsed / 1000.0);
            m_Renderer.EndFrame();
            SDL_GL_SwapWindow(m_Window);
            renderElapsed -= 1000/FRAMES_PER_SECOND;
        }
    }
}

void atContext::Exit() {
    m_Run = false;
}

atConfig atContext::GetConfig() {
    return m_Config;
}
