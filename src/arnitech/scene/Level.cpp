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

#include "Body.hpp"
#include "Level.hpp"
#include "resources/Resources.hpp"
#include "resources/PathFinder.hpp"
#include "system/Context.hpp"
#include <cstring>
#include <functional>

atLevel::atLevel() :
    m_Gravity( 0.0f, -10.0f ),
    m_PhysicsTime( 0.0f ),
    m_TimeStep( 1 / 60.0f )
{
    m_B2World = new b2World(m_Gravity);
    m_B2World->SetContactListener(this);
}

atLevel::~atLevel() {
    delete m_B2World;
}

void atLevel::StartUp() {
}

void atLevel::ShutDown() {
}

void atLevel::_StartUp() {
    StartUp();
}

void atLevel::_ShutDown() {
    ShutDown();
    for( atEntity* e : m_Entities ) {
        e->Leave(this);
        delete e;
    }
}

void atLevel::ProcessEvents() {
    SDL_Event event;
    /* Check for new events */
    while(SDL_PollEvent(&event)) {
        if( event.key.type == SDL_KEYDOWN || event.key.type == SDL_KEYUP ) { 
            atKeyEvent* kEvent =  new atKeyEvent() ;
            kEvent->m_KEType = event.key.type == SDL_KEYDOWN ? K_PRESSED : K_RELEASED;
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    kEvent->m_KCode = E_K_ESC;
                    atContext::Exit();
                    break;
                case SDLK_LEFT:
                    kEvent->m_KCode = E_K_LEFT;
                    break;
                case SDLK_RIGHT:
                    kEvent->m_KCode = E_K_RIGHT;
                    break;
                case SDLK_e:
                    kEvent->m_KCode = E_K_E;
                    break;
                case SDLK_SPACE:
                    kEvent->m_KCode = E_K_SPACE;
                    break;
            }
            m_EventManager.LaunchEvent( E_KEYBOARD, std::shared_ptr<void>(kEvent));
        } 
    }
}

void atLevel::BeginContact(b2Contact* contact) {
    atEntity*  entityA = static_cast<atEntity*>(contact->GetFixtureA()->GetUserData());
    atEntity*  entityB = static_cast<atEntity*>(contact->GetFixtureB()->GetUserData());
    if( entityA != NULL && entityB != NULL ) {
        atCollision collisionA;
        collisionA.m_Type = E_ENTER; 
        collisionA.m_Entity = entityB;
        entityA->Collide( collisionA );

        atCollision collisionB;
        collisionB.m_Type = E_ENTER; 
        collisionB.m_Entity = entityA;
        entityB->Collide( collisionB );
    }
}

void atLevel::EndContact(b2Contact* contact) {
    atEntity*  entityA = static_cast<atEntity*>(contact->GetFixtureA()->GetUserData());
    atEntity*  entityB = static_cast<atEntity*>(contact->GetFixtureB()->GetUserData());
    if( entityA != NULL && entityB != NULL ) {
        atCollision collisionA;
        collisionA.m_Type = E_LEAVE; 
        collisionA.m_Entity = entityB;
        entityA->Collide( collisionA );

        atCollision collisionB;
        collisionB.m_Type = E_LEAVE; 
        collisionB.m_Entity = entityA;
        entityB->Collide( collisionB );
    }
}

void atLevel::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}

void atLevel::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}

void atLevel::Draw( const double elapsedTime ) {
    for( atEntity* e : m_Entities ) {
        e->Draw( elapsedTime, e->Depth() );
    }
}

void atLevel::DrawDebug( const double elapsedTime ) {
    for( atEntity* e : m_Entities ) {
        e->DrawShape( elapsedTime, 256);
    }
}

void atLevel::Update( const double elapsedTime ) {
    ProcessEvents();
    for( atEntity* e : m_Entities ) {
        e->Update( elapsedTime );
    }
}

void atLevel::Insert( atEntity* entity, const atVector2f& position, const atVector2f& extent ) {
    entity->Enter(this, position, extent );
    m_Entities.push_back(entity);
}

void atLevel::Remove( atEntity* entity ) {

}

atEntity* atLevel::GetEntity( const char* name ) {
    for( atEntity* e : m_Entities ) {
        if( std::strcmp( e->Name(), name ) == 0 ) {
            return e;
        }
    }
    return NULL;
}

void atLevel::SimulatePhysics( const double elapsedTime ) {
    m_PhysicsTime+=elapsedTime;
    while(m_PhysicsTime >= m_TimeStep ) {
        //m_B2World->Step(static_cast<float32>(m_TimeStep), 6, 2);
        m_B2World->Step(static_cast<float32>(m_TimeStep), 3, 1);
        m_PhysicsTime-=m_TimeStep;
    }
}

b2World&  atLevel::B2World() {
    return *m_B2World;
}

void atLevel::RegisterListener( atEventType eventType, std::function<void( std::shared_ptr<void>)> listener ){
    m_EventManager.RegisterListener( eventType, listener );
}
void atLevel::RegisterListener( const char* eventType,std::function<void( std::shared_ptr<void>)> listener  ) {
    m_EventManager.RegisterListener( eventType, listener );
}
void atLevel::UnregisterListener( atEventType eventType, std::function<void( std::shared_ptr<void>)> listener ) {
    m_EventManager.UnregisterListener( eventType, listener );
}
void atLevel::UnregisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener  ) {
    m_EventManager.UnregisterListener( eventType, listener );
}
void atLevel::LaunchEvent( atEventType eventType, std::shared_ptr<void> data ) {
    m_EventManager.LaunchEvent( eventType, data );
}
void atLevel::LaunchEvent( const char* eventType, std::shared_ptr<void> data ) {
    m_EventManager.LaunchEvent( eventType, data );
}
