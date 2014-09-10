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

#include "Config.hpp"
#include "Level.hpp"
#include "dali/Dali.hpp"
#include "logging/Log.hpp"
#include <GL/glew.h>
#include <SDL.h>

namespace sacman {
#define LEVEL_NAME_LENGTH   32

    class Context {
        public:

            /** @brief Initializes up the system.
             *  @param configFileName The configuration file name of the engine*/
            static void StartUp( const char* configFileName );

            /** @brief Frees the system.*/
            static void ShutDown();

            /** @brief Starts the game loop.*/
            static void StartGameLoop();

            /** @brief Exits the game loop.*/
            static void Exit();

            /** Rendering **/
            static dali::Renderer m_Renderer;

            /** Logging **/
            static logging::Log* log;

            static Config GetConfig();


        private:
            /** @brief Starts SDL stuff.*/
            static void SDLStartUp();
            /** @brief Frees SDL stuff.*/
            static void SDLShutDown();

            /** @brief Processed events.*/
            static void ProcessEvents();

            /** SDL members **/
            static SDL_GLContext    m_GLcontext;
            static SDL_Window*      m_Window;

            /** Configuration **/
            static Config           m_Config;

            /** Flow control **/
            static bool             m_Run;
            static Level            m_CurrentLevel;
    };
}

#endif
