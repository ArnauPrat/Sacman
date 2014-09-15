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

#include "LoadLevel.hpp"
#include "arnitech/resources/Resources.hpp"
#include "arnitech/resources/Tiled.hpp"
#include "arnitech/scene/Entity.hpp"
#include "arnitech/scene/Level.hpp"
#include "arnitech/scene/Camera.hpp"
#include "objects/Background.hpp"
#include "objects/Character.hpp"
#include "objects/Occluder.hpp"
#include "objects/Portal.hpp"
#include <cstring>

#define CHUNKSIZE 8 

static atEntity* LoadCharacter( atLevel& level, const atTiledLevel& tiledLevel, const atTiledObject& object ) {
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
    level.Insert(character, position, extent );
    return character;
}

static atEntity* LoadPortal( atLevel& level, const atTiledLevel& tiledLevel, const atTiledObject& object ) {
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
    level.Insert(portal, position, extent );
    return portal;
}

static atEntity* LoadBox( atLevel& level, const atTiledLevel& tiledLevel, const atTiledObject& object ) {
    atVector2f position = TransformPosition( tiledLevel, object.m_X, object.m_Y+object.m_Height);
    atVector2f extent;
    if( object.m_TileId ) {
        extent = {0.5f, 0.5f};
    } else  {
        extent = {0.5f*object.m_Width / static_cast<float>(tiledLevel.m_TileWidth), 0.5f*object.m_Height / static_cast<float>(tiledLevel.m_TileHeight)};
    }
    position.m_X+=extent.m_X;
    position.m_Y+=extent.m_Y;
    atBody* body = new atBody( object.m_Name, E_STATIC);
    level.Insert(body, position, extent );
    body->AddBox({0.0f, 0.0f}, extent, E_SOLID);
    return body;
}

static atEntity* LoadOccluder( atLevel& level, const atTiledLevel& tiledLevel, const atTiledObject& object) {
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
        level.Insert(occluder, position, extent);
        return occluder;
    }
    return NULL;
}



static void LoadChunk( Background& background, const atTiledLevel& tiledLevel, const atTiledLayer& layer, const atTiledTileSet& tileSet, const atTiledCell cells[], const int numCells) {
    atVector2f* vertices = new atVector2f[numCells * 4];
    atVector2f* texCoords = new atVector2f[numCells * 4];
    unsigned short* indices = new unsigned short[numCells * 6];
    for (int k = 0; k < numCells; ++k) {
        vertices[k * 4].m_X = static_cast<float>(cells[k].m_X);
        vertices[k * 4].m_Y = static_cast<float>(tiledLevel.m_Height - (cells[k].m_Y + 1));
        vertices[k * 4 + 1].m_X = static_cast<float>(cells[k].m_X + 1);
        vertices[k * 4 + 1].m_Y = static_cast<float>(tiledLevel.m_Height - (cells[k].m_Y + 1));
        vertices[k * 4 + 2].m_X = static_cast<float>(cells[k].m_X + 1);
        vertices[k * 4 + 2].m_Y = static_cast<float>(tiledLevel.m_Height - cells[k].m_Y);
        vertices[k * 4 + 3].m_X = static_cast<float>(cells[k].m_X);
        vertices[k * 4 + 3].m_Y = static_cast<float>(tiledLevel.m_Height - cells[k].m_Y);

        atVector2f min = MinTexCoord(tileSet, cells[k].m_Tile);
        atVector2f max = MaxTexCoord(tileSet, cells[k].m_Tile);
        texCoords[k * 4].m_X = min.m_X;
        texCoords[k * 4].m_Y = min.m_Y;
        texCoords[k * 4 + 1].m_X = max.m_X;
        texCoords[k * 4 + 1].m_Y = min.m_Y;
        texCoords[k * 4 + 2].m_X = max.m_X;
        texCoords[k * 4 + 2].m_Y = max.m_Y;
        texCoords[k * 4 + 3].m_X = min.m_X;
        texCoords[k * 4 + 3].m_Y = max.m_Y;

        indices[k * 6] = k * 4;
        indices[k * 6 + 1] = k * 4 + 1;
        indices[k * 6 + 2] = k * 4 + 3;
        indices[k * 6 + 3] = k * 4 + 1;
        indices[k * 6 + 4] = k * 4 + 2;
        indices[k * 6 + 5] = k * 4 + 3;
    }

    Chunk* chunk = new Chunk();
    chunk->m_Vertices.AddData(vertices, static_cast<int>(numCells * 4));
    chunk->m_TexCoords.AddData(texCoords, static_cast<int>(numCells * 4));
    chunk->m_Indices.AddData(indices, static_cast<int>(numCells * 6));
    chunk->m_Texture = textureLoader.Load(tileSet.m_ImageName);
    chunk->m_Depth = layer.m_Id;
    atVector2f& min = chunk->m_Vertices.m_AABBMin;
    atVector2f& max = chunk->m_Vertices.m_AABBMax;
    chunk->m_Min = min;
    chunk->m_Max = max;
    background.InsertChunk(chunk);
    delete[] vertices;
    delete[] texCoords;
    delete[] indices;
}

static void Subdivide( Background& background, const atTiledLevel& tiledLevel, const atTiledLayer& layer, const atTiledTileSet& tileSet, const atTiledCell cells[], const int numCells) {
    int chunksX = static_cast<int>(std::ceil(tiledLevel.m_Width / CHUNKSIZE));
    int chunksY = static_cast<int>(std::ceil(tiledLevel.m_Width / CHUNKSIZE));
    std::vector<atTiledCell>* chunks = new std::vector<atTiledCell>[ chunksY*chunksX ];
    for (int i = 0; i < numCells; ++i) {
        int x = cells[i].m_X / CHUNKSIZE;
        int y = cells[i].m_Y / CHUNKSIZE;
        chunks[y*chunksX + x].push_back(cells[i]);
    }
    for (int i = 0; i < chunksY*chunksX; ++i) {
        if (chunks[i].size() > 0) {
            LoadChunk( background, tiledLevel, layer, tileSet, chunks[i].data(), static_cast<int>(chunks[i].size()));
        }
    }
    delete[] chunks;
}

struct Bucket {
    std::vector<atTiledCell> m_Cells;
};


static atEntity* LoadLayer( atLevel& level, const atTiledLevel& tiledLevel, const atTiledLayer& layer ) {
    Background* background = new Background("");
    Bucket* buckets = new Bucket[tiledLevel.m_NumTileSets];
    for (int j = 0; j < layer.m_TileLayer.m_NumCells; ++j) {
        int tileSetId = FindTileSet(tiledLevel, layer.m_TileLayer.m_Cells[j].m_Tile);
        buckets[tileSetId].m_Cells.push_back(layer.m_TileLayer.m_Cells[j]);
    }
    for (int j = 0; j < tiledLevel.m_NumTileSets; ++j) {
        std::vector<atTiledCell>& cells = buckets[j].m_Cells;
        if (cells.size() > 0) {
            Subdivide(*background, tiledLevel, layer, tiledLevel.m_TileSets[j], cells.data(), static_cast<int>(cells.size()));
        }
    }
    delete[] buckets;
    level.Insert( static_cast<atEntity*>(background), {0.0f, 0.0f}, {1.0f, 1.0f} );
    return background;
}


void Load( const char* fileName, atLevel& level  ) {
    atVector2f min = {FLT_MAX, FLT_MAX};
    atVector2f max = {-FLT_MAX, -FLT_MAX};
    atTiledLevel* tiledLevel = LoadTiledLevel(pathFinder.FindPath(fileName));
    for( int i = 0; i < tiledLevel->m_NumLayers; ++i ) {
        if( tiledLevel->m_Layers[i].m_Type == E_OBJECT_GROUP ) {
            atTiledLayer& layer = tiledLevel->m_Layers[i];
            for( int j = 0; j < layer.m_ObjectGroup.m_NumObjects; ++j ) {
                atTiledObject& object = layer.m_ObjectGroup.m_Objects[j];
                atEntity* entity = NULL;
                if( std::strcmp(object.m_Type, "character") == 0 ) {
                    entity = LoadCharacter( level, *tiledLevel, object );
                } else if( std::strcmp(object.m_Type, "portal") == 0 ) {
                    entity = LoadPortal( level, *tiledLevel, object );
                } else if( std::strcmp(object.m_Type, "box") == 0 ) {
                    entity = LoadBox( level, *tiledLevel, object );
                } else if( std::strcmp(object.m_Type, "occluder") == 0 ) {
                    entity = LoadOccluder( level, *tiledLevel, object );
                }
                if (entity) entity->SetDepth(i);
            }
        } else if( tiledLevel->m_Layers[i].m_Type == E_TILE_LAYER ) {
            atEntity* entity = LoadLayer( level, *tiledLevel, tiledLevel->m_Layers[i]);
            if (entity) { 
                entity->SetDepth(i);
                atVector2f extent = entity->Extent();
                atVector2f position = entity->Position();
                min.m_X = (position.m_X - extent.m_X) < min.m_X ? ( position.m_X - extent.m_X ) : min.m_X;
                min.m_Y = (position.m_Y - extent.m_Y) < min.m_Y ? ( position.m_Y - extent.m_Y ) : min.m_Y;
                max.m_X = (position.m_X + extent.m_X) > max.m_X ? ( position.m_X + extent.m_X ) : max.m_X;
                max.m_Y = (position.m_Y + extent.m_Y) > max.m_Y ? ( position.m_Y + extent.m_Y ) : max.m_Y;
            }
        }
    }
    atCamera* camera = new atCamera("camera");
    camera->SetTarget("character");
    camera->SetLimitMin( min );
    camera->SetLimitMax( max );
    level.Insert(camera, {0.0f, 0.0f}, {1.0f, 1.0f});
    DeleteTiledLevel( tiledLevel );
}

#undef CHUNKSIZE

