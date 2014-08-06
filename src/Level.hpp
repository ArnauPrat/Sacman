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
#include "objects/Character.hpp"
#include "objects/Background.hpp"
#include "objects/Entity.hpp"
#include "objects/TexturedBox.hpp"
#include <Box2D/Box2D.h>

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
            void Draw( const double elapsedTime );

            void SimulatePhysics( const double elapsedTime );

            b2World&  B2World();

        private:
            Level( const Level& );

            /** @brief Loads a level from a file.
             *  @param fileName The level's file name .*/
            void LoadLevel( const char* fileName );

            /** Event Handling **/
            EventManager    m_EventManager;

            /** Character **/
            math::Vector2i  m_Velocity;    /**< @brief The current velocity vector of the main character.*/
            float           m_Speed;       /**< @brief The speed of the main character.*/
            Character*      m_Character;   /**< @brief The main character.*/

            /** Scene **/
            Background      m_Background;
            std::vector<Entity*>    m_Entities;

            /** Physics **/
            b2Vec2          m_Gravity;      /**< @brief The physics of the scene.*/
            b2World*        m_B2World;      /**< @brief The physics world.*/
            double          m_PhysicsTime;  /**< @brief The physics time.*/
            double          m_TimeStep;     /**< @brief The physics time step.*/
    };
}
#endif

