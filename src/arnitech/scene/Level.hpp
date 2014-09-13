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

#ifndef ATLEVEL_H
#define ATLEVEL_H

#include "scene/Entity.hpp"
#include "system/EventManager.hpp"
#include <Box2D/Box2D.h>

#define ATLEVEL_CLASS(className) class className : public atLevel 
#define ATLEVEL_START_UP  virtual void StartUp();
#define ATLEVEL_SHUT_DOWN virtual void ShutDown();

#define ATLEVEL_START_UP_DEF(className)  void className::StartUp()
#define ATLEVEL_SHUT_DOWN_DEF(className) void className::ShutDown()

class atRenderer;
class atLevel : public b2ContactListener {
    friend class atContext;
    public:
    atLevel();
    virtual ~atLevel();

    /** @brief Initializes a level.*/
    virtual void StartUp();

    /** @brief Frees a level.*/
    virtual void ShutDown();

    /** @brief Draws the level.**/
    void Draw( const double elapsedTime );

    /** @brief Draws debug information.**/
    void DrawDebug( const double elapsedTime );

    /** @brief Updates the entities of the level.*/
    void Update( const double elapsedTime );

    /** @brief Inserts an entity into the level.
     *  @param position The position to insert the entity to.
     *  @param extent The extent of the entity.*/
    void Insert( atEntity* entity, const atVector2f& position, const atVector2f& extent);

    /** @brief Removes an entity from the level. */
    void Remove( atEntity* entity );

    /** @brief Gets an entity. */
    atEntity* GetEntity( const char* name );

    /** Event managing functions. **/
    void RegisterListener( atEventType eventType, std::function<void( std::shared_ptr<void>)> listener ); 
    void RegisterListener( const char* eventType,std::function<void( std::shared_ptr<void>)> listener  ); 
    void UnregisterListener( atEventType eventType, std::function<void( std::shared_ptr<void>)> listener ); 
    void UnregisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener  ); 
    void LaunchEvent( atEventType eventType, std::shared_ptr<void> data );
    void LaunchEvent( const char* eventType, std::shared_ptr<void> data );

    b2World&  B2World();
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    private:

    void _StartUp();
    void _ShutDown();

    atLevel( const atLevel& );
    void SimulatePhysics( const double elapsedTime );

    /** @brief Processes the level events.**/
    void ProcessEvents();

    /** Event Handling **/
    atEventManager    m_EventManager;

    /** Scene **/
    std::vector<atEntity*>       m_Entities;

    /** Physics **/
    b2Vec2          m_Gravity;      /**< @brief The physics of the scene.*/
    b2World*        m_B2World;      /**< @brief The physics world.*/
    double          m_PhysicsTime;  /**< @brief The physics time.*/
    double          m_TimeStep;     /**< @brief The physics time step.*/
};
#endif

