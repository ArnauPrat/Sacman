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

#include "../objects/Body.hpp"
#include "../objects/Character.hpp"
#include "../objects/Occluder.hpp"
#include "../objects/Portal.hpp"
#include "Level.hpp"
#include "renderer/Globals.hpp"
#include "resources/PathFinder.hpp"
#include "resources/Tiled.hpp"
#include "system/Context.hpp"
#include <cstring>
#include <functional>



atLevel::atLevel() :
    m_Background("background"),
    m_Gravity( 0.0f, -10.0f ),
    m_PhysicsTime( 0.0f ),
    m_TimeStep( 1 / 60.0f )
{
    m_Velocity = {0,0};
    m_Speed = 3.0f;
    m_B2World = new b2World(m_Gravity);
    m_B2World->SetContactListener(this);
}

atLevel::~atLevel() {
    delete m_B2World;
}

void atLevel::StartUp() {
    LoadLevel("test.json");
}

void atLevel::ShutDown() {
    for( atEntity* e : m_Entities ) {
        e->Leave(this);
        delete e;
    }
}

void atLevel::ProcessEvents() {
    m_Velocity = {0, 0};
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
    atEntity* character = GetEntity("character");
    if( character ) {
        atConfig config = atContext::GetConfig();
        atVector2f frustrumExtent;
        if( config.m_RendererConfig.m_RenderingMode == E_PIXEL_ALIGNED ) {
            frustrumExtent.m_X = config.m_RendererConfig.m_ViewportWidth / (2.0f*static_cast<float>(config.m_RendererConfig.m_CellWidth));
            frustrumExtent.m_Y = config.m_RendererConfig.m_ViewportHeight / (2.0f*static_cast<float>(config.m_RendererConfig.m_CellHeight));
        } else {
            frustrumExtent.m_X = config.m_RendererConfig.m_GridWidth / 2.0f;
            frustrumExtent.m_Y = config.m_RendererConfig.m_GridHeight / 2.0f;
        }
        atVector2f levelCenter = m_Background.Position();
        atVector2f levelExtent = m_Background.Extent();
        atVector2f levelLowerLeft = {levelCenter.m_X - levelExtent.m_X, levelCenter.m_Y - levelExtent.m_Y};
        levelExtent.m_X = levelExtent.m_X < frustrumExtent.m_X ? frustrumExtent.m_X : levelExtent.m_X;
        levelExtent.m_Y = levelExtent.m_Y < frustrumExtent.m_Y ? frustrumExtent.m_Y : levelExtent.m_Y;
        levelCenter = {levelLowerLeft.m_X + levelExtent.m_X, levelLowerLeft.m_Y + levelExtent.m_Y };

        atVector2f lowerLeft = {levelCenter.m_X - levelExtent.m_X + frustrumExtent.m_X, levelCenter.m_Y - levelExtent.m_Y + frustrumExtent.m_Y};
        atVector2f upperRight = {levelCenter.m_X + levelExtent.m_X - frustrumExtent.m_X, levelCenter.m_Y + levelExtent.m_Y - frustrumExtent.m_Y};
        atVector2f cameraPosition = character->Position();
        cameraPosition.m_X = cameraPosition.m_X < lowerLeft.m_X ? lowerLeft.m_X : cameraPosition.m_X;
        cameraPosition.m_X = cameraPosition.m_X > upperRight.m_X ? upperRight.m_X : cameraPosition.m_X;
        cameraPosition.m_Y = cameraPosition.m_Y < lowerLeft.m_Y ? lowerLeft.m_Y : cameraPosition.m_Y;
        atContext::m_Renderer.SetCameraPosition( cameraPosition );
    }

    m_Background.Draw( elapsedTime, 0 );
    for( atEntity* e : m_Entities ) {
        e->Draw( elapsedTime, e->Depth() );
    }
}

void atLevel::DrawDebug( const double elapsedTime ) {
    m_Background.DrawShape(elapsedTime, 256);
    for( atEntity* e : m_Entities ) {
                    e->DrawShape( elapsedTime, 256);
    }
}

void atLevel::Update( const double elapsedTime ) {
    ProcessEvents();
    for( atEntity* e : m_Entities ) {
        e->Update( elapsedTime );
    }
    SimulatePhysics( elapsedTime );

}

void atLevel::Insert( atEntity* entity, atVector2f& position, atVector2f& extent ) {
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
        m_B2World->Step(static_cast<float32>(m_TimeStep), 6, 2);
        m_PhysicsTime-=m_TimeStep;
    }
}

b2World&  atLevel::B2World() {
    return *m_B2World;
}

atEntity* atLevel::LoadCharacter( const atTiledLevel& tiledLevel, const atTiledObject& object ) {
    atVector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
    atVector2f extent;
    if( object.m_TileId ) {
        extent = {0.5f, 0.5f};
    } else  {
        extent = {0.5f*object.m_Width / static_cast<float>(tiledLevel.m_TileWidth), 0.5f*object.m_Height / static_cast<float>(tiledLevel.m_TileHeight)};
    }
    position.m_X+=extent.m_X;
    position.m_Y+=extent.m_Y;
    Character* character = new Character(object.m_Name, "character.sprite");
    character->SetDepth(2);
    Insert(character, position, extent );
    return character;
}

atEntity* atLevel::LoadPortal( const atTiledLevel& tiledLevel, const atTiledObject& object ) {
    atVector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
    atVector2f extent;
    if( object.m_TileId ) {
        extent = {0.5f, 0.5f};
    } else  {
        extent = {0.5f*object.m_Width / static_cast<float>(tiledLevel.m_TileWidth), 0.5f*object.m_Height / static_cast<float>(tiledLevel.m_TileHeight)};
    }
    position.m_X+=extent.m_X;
    position.m_Y+=extent.m_Y;
    const char* targetLevel = FindProperty(object, "target");
    const char* x = FindProperty(object,"target_x");
    const char* y = FindProperty(object,"target_y");
    atVector2f targetPosition = {0.0f, 0.0f};
    if( x ) targetPosition.m_X = static_cast<float>(atof(x));
    if( y ) targetPosition.m_Y = static_cast<float>(atof(y));
    Portal* portal = new Portal( object.m_Name, targetLevel, targetPosition );
    if( object.m_TileId > 0 ) {
        atVector2f texCoords[4];
        atTiledTileSet& tileSet = tiledLevel.m_TileSets[FindTileSet( tiledLevel, object.m_TileId )];
        atVector2f min = MinTexCoord( tileSet, object.m_TileId );  
        atVector2f max = MaxTexCoord( tileSet, object.m_TileId );  
        texCoords[0].m_X = min.m_X;
        texCoords[0].m_Y = min.m_Y;
        texCoords[1].m_X = max.m_X;
        texCoords[1].m_Y = min.m_Y;
        texCoords[2].m_X = max.m_X;
        texCoords[2].m_Y = max.m_Y;
        texCoords[3].m_X = min.m_X;
        texCoords[3].m_Y = max.m_Y;
        portal->LoadTexture( tileSet.m_ImageName, texCoords );
    }
    Insert(portal, position, extent );
    return portal;
}

atEntity* atLevel::LoadBox( const atTiledLevel& tiledLevel, const atTiledObject& object ) {
    atVector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
    atVector2f extent;
    if( object.m_TileId ) {
        extent = {0.5f, 0.5f};
    } else  {
        extent = {0.5f*object.m_Width / static_cast<float>(tiledLevel.m_TileWidth), 0.5f*object.m_Height / static_cast<float>(tiledLevel.m_TileHeight)};
    }
    position.m_X+=extent.m_X;
    position.m_Y+=extent.m_Y;
    Body* body = new Body( object.m_Name, E_STATIC);
    Insert(body, position, extent );
    body->AddBox({0.0f, 0.0f}, extent, E_SOLID);
    return body;
}

atEntity* atLevel::LoadOccluder( const atTiledLevel& tiledLevel, const atTiledObject& object ) {
    if (object.m_TileId > 0) {
        atVector2f position = TransformPosition(tiledLevel, object.m_X, object.m_Y + object.m_Height);
        atVector2f extent = { 0.5f, 0.5f };
        position.m_X += extent.m_X;
        position.m_Y += extent.m_Y;
        atVector2f texCoords[4];
        atTiledTileSet& tileSet = tiledLevel.m_TileSets[FindTileSet( tiledLevel, object.m_TileId )];
        atVector2f min = MinTexCoord( tileSet, object.m_TileId );  
        atVector2f max = MaxTexCoord( tileSet, object.m_TileId );  
        texCoords[0].m_X = min.m_X;
        texCoords[0].m_Y = min.m_Y;
        texCoords[1].m_X = max.m_X;
        texCoords[1].m_Y = min.m_Y;
        texCoords[2].m_X = max.m_X;
        texCoords[2].m_Y = max.m_Y;
        texCoords[3].m_X = min.m_X;
        texCoords[3].m_Y = max.m_Y;
        Occluder* occluder = new Occluder( object.m_Name, tileSet.m_ImageName, texCoords );
        Insert(occluder, position, extent);
        return occluder;
    }
    return NULL;
}

void atLevel::LoadLevel( const char* fileName ) {
    atTiledLevel* tiledLevel = LoadTiledLevel(pathFinder.FindPath(fileName));
    m_Background.Load(*tiledLevel);
    for( int i = 0; i < tiledLevel->m_NumLayers; ++i ) {
        if( tiledLevel->m_Layers[i].m_Type == E_OBJECT_GROUP ) {
            atTiledLayer& layer = tiledLevel->m_Layers[i];
            for( int j = 0; j < layer.m_ObjectGroup.m_NumObjects; ++j ) {
                atTiledObject& object = layer.m_ObjectGroup.m_Objects[j];
                atEntity* entity = NULL;
                if( std::strcmp(object.m_Type, "character") == 0 ) {
                    entity = LoadCharacter( *tiledLevel, object );
                } else if( std::strcmp(object.m_Type, "portal") == 0 ) {
                    entity = LoadPortal( *tiledLevel, object );
                } else if( std::strcmp(object.m_Type, "box") == 0 ) {
                    entity = LoadBox( *tiledLevel, object );
                } else if( std::strcmp(object.m_Type, "occluder") == 0 ) {
                    entity = LoadOccluder( *tiledLevel, object );
                }
                if (entity) entity->SetDepth(i);
            }
        }
    }
    DeleteTiledLevel( tiledLevel );
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
