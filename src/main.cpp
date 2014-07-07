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

#include <iostream>
#include <SDL.h>
#include "dali/dali.hpp"

SDL_GLContext glcontext;
SDL_Window *window;

void SDLStartUp() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    window = SDL_CreateWindow(
            "SDL2/OpenGL Demo", 0, 0, 640, 480, 
            SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

    // Create an OpenGL context associated with the window.
    glcontext = SDL_GL_CreateContext(window);

    // now you can make GL calls.
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}

void SDLShutDown() {
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
	SDL_Quit();
}

int main( int argc, char** argv ) {
    SDLStartUp(); 
    dali::RendererConfig config;
    config.m_ViewportWidth = 640;
    config.m_ViewportHeight = 480;
    dali::Renderer renderer( config );
    renderer.StartUp();
    renderer.ShutDown();
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            /* If a quit event has been sent */
            if (event.type == SDL_QUIT) {
                /* Quit the application */
                quit = true;
            }
        }
    }
    SDLShutDown();
    return 0;
}



