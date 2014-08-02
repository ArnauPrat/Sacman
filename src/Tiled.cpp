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
        const Json::Value layers = root["layers"];
        level->m_NumLayers = layers.size();
        level->m_Layers = new TiledLayer[level->m_NumLayers]; 
        int i = 0;
        for( Json::Value::iterator it = layers.begin(); it != layers.end(); ++it, ++i) {
            const Json::Value data = (*it)["data"];
            int numFull = 0;
            for( Json::Value::iterator it2 = data.begin(); it2 != data.end(); ++it2) {
                if( (*it2) != 0 ) {
                    numFull++;
                }
            }
            level->m_Layers[i].m_NumCells = numFull;
            level->m_Layers[i].m_Data = new TiledCell[ level->m_Layers[i].m_NumCells ];
            int j = 0;
            for( Json::Value::iterator it2 = data.begin(); it2 != data.end(); ++it2) {
                if( (*it2) != 0 ) {
                    level->m_Layers[i].m_Data[j].m_X = j / level->m_Width;
                    level->m_Layers[i].m_Data[j].m_Y = j % level->m_Width;
                    level->m_Layers[i].m_Data[j].m_Tile = (*it2).asInt();
                    j++;
                }
            }
        }

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
            level->m_TileSets[i].m_Spacing = (*it)["spacint"].asInt();
        }
        return level;
    }

    void DeleteTiledLevel( TiledLevel* tiledLevel ) {
        for( int i = 0; i < tiledLevel->m_NumLayers; ++i ) {
            delete tiledLevel->m_Layers[i].m_Data;
        }
        delete tiledLevel->m_Layers;
        delete tiledLevel->m_TileSets;
        delete tiledLevel;
    }

}
