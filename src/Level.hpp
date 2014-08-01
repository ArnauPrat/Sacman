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

#ifndef LEVEL_H
#define LEVEL_H

#include "EventManager.hpp"
#include "objects/Sprite.hpp"

namespace sacman {
    class Renderer;
    class Level {
        public:
            Level();
            ~Level();

            /** @brief Initializes a level.*/
            void StartUp();

            /** @brief Frees a level.*/
            void ShutDown();

            /** @brief Provess the level events.**/
            void ProcessEvents();

            /** @brief Draws the level.**/
            void Draw( double elapsedTime );

        private:
            Level( const Level& );

            /** Event Handling **/
            EventManager    m_EventManager;

            /** Character **/
            dali::Vector2f   m_Position;
            dali::Vector2f   m_Scale;
            dali::Vector2f   m_Velocity;
            dali::Sprite*    m_Character;

    };
}
#endif

