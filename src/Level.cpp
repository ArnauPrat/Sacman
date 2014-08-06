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

#include "Level.hpp"
#include "Context.hpp"
#include "Tiled.hpp"
#include "dali/Globals.hpp"
#include "dali/PathFinder.hpp"

namespace sacman {

    Level::Level() :
        m_Character(NULL),
        m_Gravity( 0.0f, -10.0f ),
        m_PhysicsTime( 0.0f ),
        m_TimeStep( 1 / 60.0f )
    {
        m_Velocity = {0,0};
        m_Speed = 3.0f;
        m_B2World = new b2World(m_Gravity);
    }

    Level::~Level() {
        delete m_B2World;
    }

    void Level::StartUp() {
        m_Character = new Character( *this );
        LoadLevel("test.json");
    }

    void Level::ShutDown() {
        if(m_Character)
            delete m_Character;
        for( Entity* e : m_Entities ) {
            delete e;
        }
    }

    void Level::ProcessEvents() {
        m_Velocity = {0, 0};
        SDL_Event event;
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            EventType eType = NONE;
            if( event.key.type == SDL_KEYDOWN ) { 
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        eType = K_ESC;
                        Context::Exit();
                        break;
                    case SDLK_LEFT:
                        //      eType = K_LEFT;
                        m_Velocity.m_X = -1; 
                        if( !m_Character->IsAnimationRunning( "WalkLeft" ) ) {
                            m_Character->LaunchAnimation( "WalkLeft", 1.0f, false );
                        }
                        break;
                    case SDLK_RIGHT:
                        //     eType = K_RIGHT;
                        m_Velocity.m_X = 1; 
                        if( !m_Character->IsAnimationRunning( "WalkRight" ) ) {
                            m_Character->LaunchAnimation( "WalkRight", 1.0f, false );
                        }
                        break;
                }
            } else if(event.key.type == SDL_KEYUP) {
                m_Character->StopAnimation();
            }
            if( eType != NONE ) m_EventManager.LaunchEvent( eType, NULL );
        }
    }

    void Level::Draw( const double elapsedTime ) {
        //        m_Character.m_Position.m_X = m_Character.m_Position.m_X + m_Speed*m_Velocity.m_X*elapsedTime;
        //       m_Character.m_Position.m_Y = m_Character.m_Position.m_Y + m_Speed*m_Velocity.m_Y*elapsedTime;
        m_Character->Draw( elapsedTime, 2 );
        m_Background.Draw( elapsedTime );
        m_Character->DrawShape();

        for( Entity* e : m_Entities ) {
            e->Draw( elapsedTime, 0);
            e->DrawShape( elapsedTime, 10);
        }
        /*        for( b2Body* body = m_B2World.GetBodyList(); body; body = body->GetNext()) {
                  for( b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() ) {
                  const b2AABB& aabb = fixture->GetAABB(0);
                  b2Vec2 center = aabb.GetCenter();
                  b2Vec2 extents = aabb.GetExtents();
                  Context::m_Renderer.DrawBox( { center.x - extents.x, center.y - extents.y }, { extents.x*2.0f, extents.y*2.0f } , {1.0f, 0.0f, 0.0f, 1.0f} );
                  }
                  }
                  */
    }

    void Level::SimulatePhysics( const double elapsedTime ) {
        m_PhysicsTime+=elapsedTime;
        while(m_PhysicsTime >= m_TimeStep ) {
            m_B2World->Step(m_TimeStep, 6, 2);
            m_PhysicsTime-=m_TimeStep;
        }
    }

    b2World&  Level::B2World() {
        return *m_B2World;
    }

    void Level::LoadLevel( const char* fileName ) {
        m_Character->Load("character.sprite");
        m_Character->m_Position.m_X = 0;
        m_Character->m_Position.m_Y = 3;
        TiledLevel* tiledLevel = LoadTiledLevel(dali::pathFinder.FindPath(fileName));
        m_Background.Load(*tiledLevel);
        for( int i = 0; i < tiledLevel->m_NumLayers; ++i ) {
            if( tiledLevel->m_Layers[i].m_Type == OBJECT_GROUP ) {
                TiledLayer& layer = tiledLevel->m_Layers[i];
                for( int j = 0; j < layer.m_ObjectGroup.m_NumObjects; ++j ) {
                    TiledObject& object = layer.m_ObjectGroup.m_Objects[j];
                    TexturedBox* box = NULL; 
                    math::Vector2f position = TransformPosition( *tiledLevel, object.m_X, object.m_Y+object.m_Height);
                    if(object.m_TileId) {
                        box = new TexturedBox( object.m_Name, *this, position, {1.0f, 1.0f} );  
                        math::Vector2f texCoords[4];
                        TiledTileSet& tileSet = tiledLevel->m_TileSets[FindTileSet( *tiledLevel, object.m_TileId )];
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
                        box->LoadTexture( tileSet.m_ImageName, texCoords );
                    } else {
                        box = new TexturedBox( object.m_Name, *this, position, {object.m_Width / static_cast<float>(tiledLevel->m_TileWidth) , 
                                object.m_Height / static_cast<float>(tiledLevel->m_TileHeight)} );  
                    }
                    box->SetBoxShape();
                    m_Entities.push_back(box);
                }
            }
        }
        DeleteTiledLevel( tiledLevel );
    }
}
