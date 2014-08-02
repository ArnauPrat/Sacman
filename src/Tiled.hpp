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

namespace sacman {


    struct TiledCell {
        int m_X;
        int m_Y;
        int m_Tile;
    };

    struct TiledLayer {
        int        m_NumCells;
        TiledCell* m_Data;
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

}

#endif
