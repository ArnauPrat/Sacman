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

#ifndef TILED_H
#define TILED_H

#include "math/Types.hpp"
#include <map>
#include <string>

namespace sacman {

#define OBJECT_NAME_LENGTH 32
#define OBJECT_TYPE_NAME_LENGTH 32
#define OBJECT_PROPERTY_NAME_LENGTH 32
#define OBJECT_PROPERTY_VALUE_LENGTH 32


    enum LayerType {
        OBJECT_GROUP,
        TILE_LAYER
    };

    struct TiledCell {
        int m_X;
        int m_Y;
        int m_Tile;
    };

    struct ObjectProperty { 
        char m_Name[OBJECT_PROPERTY_NAME_LENGTH];
        char m_Value[OBJECT_PROPERTY_VALUE_LENGTH];
    };

    struct TiledObject {
        int m_TileId;
        char m_Name[OBJECT_NAME_LENGTH];
        int m_X;
        int m_Y;
        int m_Width;
        int m_Height;
        char m_Type[OBJECT_TYPE_NAME_LENGTH];
        ObjectProperty* m_Properties; 
        int m_NumProperties;
    };

    struct TiledLayer {
        LayerType  m_Type;
        union {
            struct {
                int        m_NumCells;
                TiledCell* m_Cells;
            } m_TileLayer;
            struct {
                int        m_NumObjects;
                TiledObject* m_Objects;
            } m_ObjectGroup;
        };
    };

    struct TiledTileSet {
        int m_FirstId;
        char m_ImageName[128];
        int m_ImageWidth;
        int m_ImageHeight;
        int m_Spacing;
        int m_TileWidth;
        int m_TileHeight;
    };

    struct TiledLevel {
        int m_TileWidth;
        int m_TileHeight;
        int m_Width;
        int m_Height;
        int m_NumTileSets;
        int m_NumLayers;
        TiledTileSet* m_TileSets;
        TiledLayer* m_Layers;
    };

    TiledLevel* LoadTiledLevel( const char* fileName );
    void DeleteTiledLevel( TiledLevel* tiledLevel );
    int FindTileSet( const TiledLevel& tiledLevel, int id );
    math::Vector2f MinTexCoord( const TiledTileSet& tileSet, int id );
    math::Vector2f MaxTexCoord( const TiledTileSet& tileSet, int id );
    math::Vector2f TransformPosition( const TiledLevel& level, int x, int y );
    const char* FindProperty( const TiledObject& object, const char* propertyName );
}

#endif
