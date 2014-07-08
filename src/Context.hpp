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

#ifndef SACMAN_CONTEXT_H
#define SACMAN_CONTEXT_H

#include <GL/glew.h>
#include <SDL.h>

namespace sacman {

    class Context {
        public:

            /** @brief Initializes up the system.*/
            static void StartUp();

            /** @brief Frees the system.*/
            static void ShutDown();

        private:
            /** @brief Starts SDL stuff.*/
            static void SDLStartUp();
            /** @brief Frees SDL stuff.*/
            static void SDLShutDown();

            static SDL_GLContext    m_GLcontext;
            static SDL_Window*      m_Window;
    };
}

#endif
