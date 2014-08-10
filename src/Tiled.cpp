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

#include "Tiled.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <streambuf>
#include <string>

namespace sacman {

    TiledLevel* LoadTiledLevel( const char* fileName ) {
        std::ifstream t(fileName);
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        Json::Value root;
        Json::Reader reader;
        bool parsedSuccess = reader.parse( str, root, false );
        if(!parsedSuccess) {
            std::cout << "Failed to parse Tiled .json file: " << fileName << std::endl 
                << reader.getFormatedErrorMessages()
                << std::endl;
            assert(false);
        }
        TiledLevel* level = new TiledLevel();
        level->m_TileWidth = root["tilewidth"].asInt();
        level->m_TileHeight = root["tileheight"].asInt();
        level->m_Width = root["width"].asInt();
        level->m_Height = root["height"].asInt();
        const Json::Value& layers = root["layers"];
        level->m_Layers = new TiledLayer[layers.size()]; 
        int i = 0;
        for( Json::Value::iterator it = layers.begin(); it != layers.end(); ++it) {
            const char* type = (*it)["type"].asCString();
            if( std::strcmp(type,"tilelayer") == 0 ) {
                level->m_Layers[i].m_Type = TILE_LAYER;
                const Json::Value& data = (*it)["data"];
                int numFull = 0;
                for( Json::Value::iterator it2 = data.begin(); it2 != data.end(); ++it2) {
                    if( (*it2) != 0 ) {
                        numFull++;
                    }
                }
                level->m_Layers[i].m_TileLayer.m_NumCells = numFull;
                level->m_Layers[i].m_TileLayer.m_Cells = new TiledCell[ level->m_Layers[i].m_TileLayer.m_NumCells ];
                int j = 0;
                int k = 0;
                for( Json::Value::iterator it2 = data.begin(); it2 != data.end(); ++it2, ++j) {
                    if( (*it2) != 0 ) {
                        level->m_Layers[i].m_TileLayer.m_Cells[k].m_X = j % level->m_Width;
                        level->m_Layers[i].m_TileLayer.m_Cells[k].m_Y = j / level->m_Width;
                        level->m_Layers[i].m_TileLayer.m_Cells[k].m_Tile = (*it2).asInt();
                        k++;
                    }
                }
                i++;
            } else if( std::strcmp( type, "objectgroup" ) == 0 ) {
                level->m_Layers[i].m_Type = OBJECT_GROUP;
                const Json::Value& objects = (*it)["objects"];
                level->m_Layers[i].m_ObjectGroup.m_Objects = new TiledObject[objects.size()];
                int j = 0;
                for( Json::Value::iterator it2 = objects.begin(); it2 != objects.end(); ++it2) {
                    TiledObject& object = level->m_Layers[i].m_ObjectGroup.m_Objects[j];
                    if( (*it2).isMember("gid") ) {
                        level->m_Layers[i].m_ObjectGroup.m_Objects[j].m_TileId = (*it2)["gid"].asInt();
                    } else {
                        level->m_Layers[i].m_ObjectGroup.m_Objects[j].m_TileId = 0;
                    }
                    const char* name = (*it2)["name"].asCString();
                    assert(std::strlen(name) < OBJECT_NAME_LENGTH);
                    std::strcpy(level->m_Layers[i].m_ObjectGroup.m_Objects[j].m_Name, name);
                    const char* type = (*it2)["type"].asCString();
                    assert(std::strlen(type) < OBJECT_TYPE_NAME_LENGTH);
                    std::strcpy(level->m_Layers[i].m_ObjectGroup.m_Objects[j].m_Type, type);
                    object.m_X = (*it2)["x"].asInt();
                    object.m_Y = (*it2)["y"].asInt();
                    object.m_Width = (*it2)["width"].asInt();
                    object.m_Height = (*it2)["height"].asInt();
                    // Reading object properties
                    object.m_Properties = NULL;
                    const Json::Value& properties = (*it2)["properties"];
                    object.m_NumProperties = properties.size();
                    const Json::Value::Members& names = properties.getMemberNames();
                    if( object.m_NumProperties > 0 ) {
                        object.m_Properties = new ObjectProperty[object.m_NumProperties];
                    }
                    int k = 0;
                    for( auto itP = names.begin(); itP != names.end(); ++itP, ++k) {
                        assert( std::strlen((*itP).c_str()) < OBJECT_PROPERTY_NAME_LENGTH );
                        assert( std::strlen(properties[(*itP)].asCString()) < OBJECT_PROPERTY_VALUE_LENGTH );
                        std::strcpy(object.m_Properties[k].m_Name, (*itP).c_str());
                        std::strcpy(object.m_Properties[k].m_Value, properties[(*itP)].asCString());
                    }
                    ++j;
                }
                level->m_Layers[i].m_ObjectGroup.m_NumObjects = j;
                i++;
            } else {
                assert(false);
            }
        }
        level->m_NumLayers = i;

        const Json::Value tileSets = root["tilesets"];
        level->m_NumTileSets = tileSets.size();
        level->m_TileSets = new TiledTileSet[level->m_NumTileSets];
        i = 0; 
        for( Json::Value::iterator it = tileSets.begin(); it != tileSets.end(); ++it, ++i) {
            level->m_TileSets[i].m_FirstId = (*it)["firstgid"].asInt();
            const char* imageName = (*it)["image"].asCString();
            const char* lastSlash = std::strrchr(imageName, '/');
            assert( std::strlen(++lastSlash) < 128 );
            std::strcpy(level->m_TileSets[i].m_ImageName, lastSlash);
            level->m_TileSets[i].m_ImageWidth = (*it)["imagewidth"].asInt();
            level->m_TileSets[i].m_ImageHeight = (*it)["imageheight"].asInt();
            level->m_TileSets[i].m_TileHeight = (*it)["tileheight"].asInt();
            level->m_TileSets[i].m_TileWidth = (*it)["tilewidth"].asInt();
            level->m_TileSets[i].m_Spacing = (*it)["spacing"].asInt();
        }
        return level;
    }

    void DeleteTiledLevel( TiledLevel* tiledLevel ) {
        for( int i = 0; i < tiledLevel->m_NumLayers; ++i ) {
            if( tiledLevel->m_Layers[i].m_Type == TILE_LAYER ) {
                delete tiledLevel->m_Layers[i].m_TileLayer.m_Cells;
            } else {
                for( int j = 0; j < tiledLevel->m_Layers[i].m_ObjectGroup.m_NumObjects; ++j ) {
                    if(tiledLevel->m_Layers[i].m_ObjectGroup.m_Objects[j].m_Properties)
                        delete tiledLevel->m_Layers[i].m_ObjectGroup.m_Objects[j].m_Properties;
                }
                delete tiledLevel->m_Layers[i].m_ObjectGroup.m_Objects;
            }
        }
        delete tiledLevel->m_Layers;
        delete tiledLevel->m_TileSets;
        delete tiledLevel;
    }

    int FindTileSet( const TiledLevel& tiledLevel, int id ) {
        for( int i = 0; i < tiledLevel.m_NumTileSets - 1; ++i ) {
            if( tiledLevel.m_TileSets[i].m_FirstId <= id && id < tiledLevel.m_TileSets[i+1].m_FirstId ) {
                return i;
            }
        } 
        return tiledLevel.m_NumTileSets - 1;
    }

    math::Vector2f MinTexCoord( const TiledTileSet& tileSet, int id ) {
        int tilesX = tileSet.m_ImageWidth / (tileSet.m_TileWidth + tileSet.m_Spacing);
        int tilesY = tileSet.m_ImageHeight / (tileSet.m_TileHeight + tileSet.m_Spacing); 
        int x = (id - 1) % (tilesX);
        int y = (id - 1) / (tilesX);
        int pixelX = x*(tileSet.m_TileWidth+tileSet.m_Spacing);
        int pixelY = (tilesY-(y))*(tileSet.m_TileHeight+tileSet.m_Spacing) - tileSet.m_TileHeight;
        return { pixelX / (float)tileSet.m_ImageWidth, pixelY / (float)tileSet.m_ImageHeight };
    };

    math::Vector2f MaxTexCoord( const TiledTileSet& tileSet, int id ) {
        int tilesX = tileSet.m_ImageWidth / (tileSet.m_TileWidth + tileSet.m_Spacing);
        int tilesY = tileSet.m_ImageHeight / (tileSet.m_TileHeight + tileSet.m_Spacing); 
        int x = (id - 1) % (tilesX);
        int y = (id - 1) / (tilesX);
        int pixelX = x*(tileSet.m_TileWidth+tileSet.m_Spacing)+tileSet.m_TileWidth;
        int pixelY = (tilesY - y)*(tileSet.m_TileHeight+tileSet.m_Spacing);
        return { pixelX / (float)tileSet.m_ImageWidth, pixelY / (float)tileSet.m_ImageHeight };
    };

    math::Vector2f TransformPosition( const TiledLevel& level, int x, int y ) {
        math::Vector2f position = { x/ static_cast<float>(level.m_TileWidth), (level.m_Height*level.m_TileHeight-y) / static_cast<float>(level.m_TileHeight) };
        return position;
    }

    const char* FindProperty( const TiledObject& object, const char* propertyName ) {
        for( int i = 0; i < object.m_NumProperties; ++i ) {
            if(std::strcmp(object.m_Properties[i].m_Name, propertyName) == 0)
                   return object.m_Properties[i].m_Value;
        }
        return NULL;
    }
}
