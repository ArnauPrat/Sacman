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

#include "Context.hpp"
#include "Level.hpp"
#include "Tiled.hpp"
#include "dali/Globals.hpp"
#include "dali/PathFinder.hpp"
#include "objects/Character.hpp"
#include "objects/Portal.hpp"
#include "objects/Body.hpp"
#include "objects/Occluder.hpp"
#include <cstring>
#include <functional>


namespace sacman {

    Level::Level() :
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

    Level::~Level() {
        delete m_B2World;
    }

    void Level::StartUp() {
        LoadLevel("test.json");
    }

    void Level::ShutDown() {
        for( Entity* e : m_Entities ) {
            e->Leave(this);
            delete e;
        }
    }

    void Level::ProcessEvents() {
        m_Velocity = {0, 0};
        SDL_Event event;
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            if( event.key.type == SDL_KEYDOWN || event.key.type == SDL_KEYUP ) { 
                KeyEvent* kEvent =  new KeyEvent() ;
                kEvent->m_KEType = event.key.type == SDL_KEYDOWN ? K_PRESSED : K_RELEASED;
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        kEvent->m_KCode = K_ESC;
                        Context::Exit();
                        break;
                    case SDLK_LEFT:
                        kEvent->m_KCode = K_LEFT;
                        break;
                    case SDLK_RIGHT:
                        kEvent->m_KCode = K_RIGHT;
                        break;
                    case SDLK_SPACE:
                        kEvent->m_KCode = K_SPACE;
                        break;
                }
                m_EventManager.LaunchEvent( KEYBOARD, std::shared_ptr<void>(kEvent));
            } 
        }
    }

    void Level::BeginContact(b2Contact* contact) {
        Entity*  entityA = static_cast<Entity*>(contact->GetFixtureA()->GetUserData());
        Entity*  entityB = static_cast<Entity*>(contact->GetFixtureB()->GetUserData());
        if( entityA != NULL && entityB != NULL ) {
            Collision collisionA;
            collisionA.m_Type = E_ENTER; 
            collisionA.m_Entity = entityB;
            entityA->Collide( collisionA );

            Collision collisionB;
            collisionB.m_Type = E_ENTER; 
            collisionB.m_Entity = entityA;
            entityB->Collide( collisionB );
        }
    }

    void Level::EndContact(b2Contact* contact) {
        Entity*  entityA = static_cast<Entity*>(contact->GetFixtureA()->GetUserData());
        Entity*  entityB = static_cast<Entity*>(contact->GetFixtureB()->GetUserData());
        if( entityA != NULL && entityB != NULL ) {
            Collision collisionA;
            collisionA.m_Type = E_LEAVE; 
            collisionA.m_Entity = entityB;
            entityA->Collide( collisionA );

            Collision collisionB;
            collisionB.m_Type = E_LEAVE; 
            collisionB.m_Entity = entityA;
            entityB->Collide( collisionB );
        }
    }

    void Level::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    }

    void Level::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    }

    void Level::Draw( const double elapsedTime ) {
        Entity* character = GetEntity("character");
        if( character ) {
//            std::cout << levelCenter.m_X << " " << levelCenter.m_Y << std::endl;
 //           std::cout << levelExtent.m_X << " " << levelExtent.m_Y << std::endl;
            Config config = Context::GetConfig();
            math::Vector2f frustrumExtent;
            if( config.m_RendererConfig.m_RenderingMode == dali::PIXEL_ALIGNED ) {
                frustrumExtent.m_X = config.m_RendererConfig.m_ViewportWidth / (2.0f*static_cast<float>(config.m_RendererConfig.m_CellWidth));
                frustrumExtent.m_Y = config.m_RendererConfig.m_ViewportHeight / (2.0f*static_cast<float>(config.m_RendererConfig.m_CellHeight));
            } else {
                frustrumExtent.m_X = config.m_RendererConfig.m_GridWidth / 2.0f;
                frustrumExtent.m_Y = config.m_RendererConfig.m_GridHeight / 2.0f;
            }
//            std::cout << frustrumExtent.m_X << " " << frustrumExtent.m_Y << std::endl;
            math::Vector2f levelCenter = m_Background.Position();
            math::Vector2f levelExtent = m_Background.Extent();
            math::Vector2f levelLowerLeft = {levelCenter.m_X - levelExtent.m_X, levelCenter.m_Y - levelExtent.m_Y};
            levelExtent.m_X = levelExtent.m_X < frustrumExtent.m_X ? frustrumExtent.m_X : levelExtent.m_X;
            levelExtent.m_Y = levelExtent.m_Y < frustrumExtent.m_Y ? frustrumExtent.m_Y : levelExtent.m_Y;
            levelCenter = {levelLowerLeft.m_X + levelExtent.m_X, levelLowerLeft.m_Y + levelExtent.m_Y };

            math::Vector2f lowerLeft = {levelCenter.m_X - levelExtent.m_X + frustrumExtent.m_X, levelCenter.m_Y - levelExtent.m_Y + frustrumExtent.m_Y};
            math::Vector2f upperRight = {levelCenter.m_X + levelExtent.m_X - frustrumExtent.m_X, levelCenter.m_Y + levelExtent.m_Y - frustrumExtent.m_Y};
            math::Vector2f cameraPosition = character->Position();
            //std::cout << "level center: " << levelCenter.m_X << " " << levelCenter.m_Y << std::endl;
            //std::cout << "frustrum: " << frustrumExtent.m_X << " " << frustrumExtent.m_Y << std::endl;
            //std::cout << "lower: " << lowerLeft.m_X << " " << lowerLeft.m_Y << std::endl;
            //std::cout << "upper: " << upperRight.m_X << " " << upperRight.m_Y << std::endl;
            cameraPosition.m_X = cameraPosition.m_X < lowerLeft.m_X ? lowerLeft.m_X : cameraPosition.m_X;
            cameraPosition.m_X = cameraPosition.m_X > upperRight.m_X ? upperRight.m_X : cameraPosition.m_X;
            cameraPosition.m_Y = cameraPosition.m_Y < lowerLeft.m_Y ? lowerLeft.m_Y : cameraPosition.m_Y;
//            cameraPosition.m_Y = cameraPosition.m_Y > upperRight.m_Y ? upperRight.m_Y : cameraPosition.m_Y;
            //std::cout << "position: " << cameraPosition.m_X << " " << cameraPosition.m_Y << std::endl;
            Context::m_Renderer.SetCameraPosition( cameraPosition );
        }

        m_Background.Draw( elapsedTime, 0 );
        for( Entity* e : m_Entities ) {
            e->Draw( elapsedTime, e->Depth() );
        }
    }

    void Level::DrawDebug( const double elapsedTime ) {
        for( Entity* e : m_Entities ) {
            e->DrawShape( elapsedTime, 10);
        }
    }

    void Level::Update( const double elapsedTime ) {
        ProcessEvents();
        for( Entity* e : m_Entities ) {
            e->Update( elapsedTime );
        }
        SimulatePhysics( elapsedTime );

    }

    void Level::Insert( Entity* entity, math::Vector2f& position, math::Vector2f& extent ) {
        entity->Enter(this, position, extent );
        m_Entities.push_back(entity);
    }

    void Level::Remove( Entity* entity ) {

    }

    Entity* Level::GetEntity( const char* name ) {
        for( Entity* e : m_Entities ) {
            if( std::strcmp( e->Name(), name ) == 0 ) {
                return e;
            }
        }
        return NULL;
    }

    void Level::SimulatePhysics( const double elapsedTime ) {
        m_PhysicsTime+=elapsedTime;
        while(m_PhysicsTime >= m_TimeStep ) {
            m_B2World->Step(static_cast<float32>(m_TimeStep), 6, 2);
            m_PhysicsTime-=m_TimeStep;
        }
    }

    b2World&  Level::B2World() {
        return *m_B2World;
    }

    void Level::LoadCharacter( const TiledLevel& tiledLevel, const TiledObject& object ) {
        math::Vector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
        math::Vector2f extent;
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
    }

    void Level::LoadPortal( const TiledLevel& tiledLevel, const TiledObject& object ) {
        math::Vector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
        math::Vector2f extent;
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
        math::Vector2f targetPosition = {0.0f, 0.0f};
        if( x ) targetPosition.m_X = static_cast<float>(atof(x));
        if( y ) targetPosition.m_Y = static_cast<float>(atof(y));
        Portal* portal = new Portal(object.m_Name, targetLevel, targetPosition );
        if( object.m_TileId > 0 ) {
            math::Vector2f texCoords[4];
            TiledTileSet& tileSet = tiledLevel.m_TileSets[FindTileSet( tiledLevel, object.m_TileId )];
            math::Vector2f min = MinTexCoord( tileSet, object.m_TileId );  
            math::Vector2f max = MaxTexCoord( tileSet, object.m_TileId );  
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
    }

    void Level::LoadBox( const TiledLevel& tiledLevel, const TiledObject& object ) {
        math::Vector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
        math::Vector2f extent;
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
    }

    void Level::LoadOccluder( const TiledLevel& tiledLevel, const TiledObject& object ) {
        if (object.m_TileId > 0) {
            math::Vector2f position = TransformPosition(tiledLevel, object.m_X, object.m_Y + object.m_Height);
            math::Vector2f extent = { 0.5f, 0.5f };
            position.m_X += extent.m_X;
            position.m_Y += extent.m_Y;
            math::Vector2f texCoords[4];
            TiledTileSet& tileSet = tiledLevel.m_TileSets[FindTileSet( tiledLevel, object.m_TileId )];
            math::Vector2f min = MinTexCoord( tileSet, object.m_TileId );  
            math::Vector2f max = MaxTexCoord( tileSet, object.m_TileId );  
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
        }
    }

    void Level::LoadLevel( const char* fileName ) {
        TiledLevel* tiledLevel = LoadTiledLevel(dali::pathFinder.FindPath(fileName));
        m_Background.Load(*tiledLevel);
        for( int i = 0; i < tiledLevel->m_NumLayers; ++i ) {
            if( tiledLevel->m_Layers[i].m_Type == OBJECT_GROUP ) {
                TiledLayer& layer = tiledLevel->m_Layers[i];
                for( int j = 0; j < layer.m_ObjectGroup.m_NumObjects; ++j ) {
                    TiledObject& object = layer.m_ObjectGroup.m_Objects[j];
                    if( std::strcmp(object.m_Type, "character") == 0 ) {
                        LoadCharacter( *tiledLevel, object );
                    } else if( std::strcmp(object.m_Type, "portal") == 0 ) {
                        LoadPortal( *tiledLevel, object );
                    } else if( std::strcmp(object.m_Type, "box") == 0 ) {
                        LoadBox( *tiledLevel, object );
                    } else if( std::strcmp(object.m_Type, "occluder") == 0 ) {
                        LoadOccluder( *tiledLevel, object );
                    }
                }
            }
        }
        DeleteTiledLevel( tiledLevel );
    }

    void Level::RegisterListener( EventType eventType, std::function<void( std::shared_ptr<void>)> listener ){
        m_EventManager.RegisterListener( eventType, listener );
    }
    void Level::RegisterListener( const char* eventType,std::function<void( std::shared_ptr<void>)> listener  ) {
        m_EventManager.RegisterListener( eventType, listener );
    }
    void Level::UnregisterListener( EventType eventType, std::function<void( std::shared_ptr<void>)> listener ) {
        m_EventManager.UnregisterListener( eventType, listener );
    }
    void Level::UnregisterListener( const char* eventType, std::function<void( std::shared_ptr<void>)> listener  ) {
        m_EventManager.UnregisterListener( eventType, listener );
    }
    void Level::LaunchEvent( EventType eventType, std::shared_ptr<void> data ) {
        m_EventManager.LaunchEvent( eventType, data );
    }
    void Level::LaunchEvent( const char* eventType, std::shared_ptr<void> data ) {
        m_EventManager.LaunchEvent( eventType, data );
    }
}
