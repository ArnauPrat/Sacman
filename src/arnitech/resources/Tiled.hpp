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

#ifndef ATTILED_H
#define ATTILED_H

#include "math/Types.hpp"
#include <map>
#include <string>


#define ATTILED_OBJECT_NAME_LENGTH 32
#define ATTILED_OBJECT_TYPE_NAME_LENGTH 32
#define ATTILED_OBJECT_PROPERTY_NAME_LENGTH 32
#define ATTILED_OBJECT_PROPERTY_VALUE_LENGTH 32


enum atLayerType {
    E_OBJECT_GROUP,
    E_TILE_LAYER
};

struct atTiledCell {
    int m_X;
    int m_Y;
    int m_Tile;
};

struct atTiledObjectProperty { 
    char m_Name[ATTILED_OBJECT_PROPERTY_NAME_LENGTH];
    char m_Value[ATTILED_OBJECT_PROPERTY_VALUE_LENGTH];
};

struct atTiledObject {
    int m_TileId;
    char m_Name[ATTILED_OBJECT_NAME_LENGTH];
    int m_X;
    int m_Y;
    int m_Width;
    int m_Height;
    char m_Type[ATTILED_OBJECT_TYPE_NAME_LENGTH];
    atTiledObjectProperty* m_Properties; 
    int m_NumProperties;
};

struct atTiledLayer {
    atLayerType   m_Type;
    int         m_Id;
    union {
        struct {
            int        m_NumCells;
            atTiledCell* m_Cells;
        } m_TileLayer;
        struct {
            int        m_NumObjects;
            atTiledObject* m_Objects;
        } m_ObjectGroup;
    };
};

struct atTiledTileSet {
    int m_FirstId;
    char m_ImageName[128];
    int m_ImageWidth;
    int m_ImageHeight;
    int m_Spacing;
    int m_TileWidth;
    int m_TileHeight;
};

struct atTiledLevel {
    int m_TileWidth;
    int m_TileHeight;
    int m_Width;
    int m_Height;
    int m_NumTileSets;
    int m_NumLayers;
    atTiledTileSet* m_TileSets;
    atTiledLayer* m_Layers;
};

atTiledLevel* LoadTiledLevel( const char* fileName );
void DeleteTiledLevel( atTiledLevel* tiledLevel );
int FindTileSet( const atTiledLevel& tiledLevel, int id );
atVector2f MinTexCoord( const atTiledTileSet& tileSet, int id );
atVector2f MaxTexCoord( const atTiledTileSet& tileSet, int id );
atVector2f TransformPosition( const atTiledLevel& level, int x, int y );
const char* FindProperty( const atTiledObject& object, const char* propertyName );

#endif
