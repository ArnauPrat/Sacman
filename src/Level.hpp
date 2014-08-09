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
#include "objects/Background.hpp"
#include "objects/Entity.hpp"
#include <Box2D/Box2D.h>
#include "Tiled.hpp"

namespace sacman {
    class Renderer;
    class Level : public b2ContactListener {
        public:
            Level();
            ~Level();

            /** @brief Initializes a level.*/
            void StartUp();

            /** @brief Frees a level.*/
            void ShutDown();

            /** @brief Draws the level.**/
            void Draw( const double elapsedTime );

            /** @brief Draws debug information.**/
            void DrawDebug( const double elapsedTime );
            
            /** @brief Updates the entities of the level.*/
            void Update( const double elapsedTime );

            void Insert( Entity* entity );
            void Remove( Entity* entity );

            /** Event managing functions. **/
            void RegisterListener( EventType eventType, std::function<void( std::shared_ptr<void>)> listener ); 
            void RegisterListener( const char* eventType,std::function<void( std::shared_ptr<void>)> listener  ); 
            void UnregisterListener( EventType eventType, std::function<void( std::shared_ptr<void>)> listener ); 
            void UnregisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener  ); 
            void LaunchEvent( EventType eventType, std::shared_ptr<void> data );
            void LaunchEvent( const char* eventType, std::shared_ptr<void> data );

            b2World&  B2World();
            void BeginContact(b2Contact* contact);
            void EndContact(b2Contact* contact);
            void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
            void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);


        private:
            Level( const Level& );
            void SimulatePhysics( const double elapsedTime );

            /** @brief Processes the level events.**/
            void ProcessEvents();

            void LoadCharacter( const TiledLevel& level, const TiledObject& object );
            void LoadPortal( const TiledLevel& level, const TiledObject& object );
            void LoadBox( const TiledLevel& level, const TiledObject& object );

            /** @brief Loads a level from a file.
             *  @param fileName The level's file name .*/
            void LoadLevel( const char* fileName );

            /** Event Handling **/
            EventManager    m_EventManager;

            /** Character **/
            math::Vector2i  m_Velocity;    /**< @brief The current velocity vector of the main character.*/
            float           m_Speed;       /**< @brief The speed of the main character.*/

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

