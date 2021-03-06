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

#include "common/Platform.hpp"
#include "common/Log.hpp"
#include "common/Macros.hpp"
#include "scene/Level.hpp"
#include "system/Config.hpp"
#include <GL/glew.h>
#include <SDL.h>

#define ATLEVEL_NAME_LENGTH   32

class atContext {
    ATNON_INSTANTIABLE(atContext);
    public:

        /** @brief Initializes up the system.
         *  @param configFileName The configuration file name of the engine*/
        ATEXPORT static void StartUp( const char* configFileName );

        /** @brief Frees the system.*/
        ATEXPORT static void ShutDown();

        /** @brief Starts the game loop.*/
        ATEXPORT static void StartGameLoop();

        /** @brief Exits the game loop.*/
        ATEXPORT static void Exit();

        /** Rendering **/
        ATEXPORT static atRenderer m_Renderer;

        /** Logging **/
        ATEXPORT static atLog* log;

        ATEXPORT static atConfig GetConfig();

        ATEXPORT static void PushLevel( atLevel* level );

        ATEXPORT static void PopLevel();


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
        static atConfig         m_Config;

        /** Flow control **/
        static bool             m_Run;
        static atLevel*         m_CurrentLevel;
};

#endif
