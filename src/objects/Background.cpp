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

#include "Background.hpp"
#include "Context.hpp"
#include "Tiled.hpp"
#include "dali/Globals.hpp"
#include "math/Types.hpp"
#include <cassert>
#include <vector>

namespace sacman {

    Background::Background() {
    }

    Background::~Background() {
    }

    struct Bucket {
        std::vector<TiledCell> m_Cells;
    };

    static int FindTileSet( const TiledLevel& tiledLevel, int id ) {
        for( int i = 0; i < tiledLevel.m_NumTileSets - 1; ++i ) {
            if( tiledLevel.m_TileSets[i].m_FirstId <= id && id < tiledLevel.m_TileSets[i+1].m_FirstId ) {
                return i;
            }
        } 
        return tiledLevel.m_NumTileSets - 1;
    }

    static math::Vector2f MinTexCoord( const TiledTileSet& tileSet, int id ) {
        int x = id / (tileSet.m_ImageWidth / tileSet.m_TileWidth);
        int y = id % (tileSet.m_ImageWidth / tileSet.m_TileWidth);
        int pixelX = x*(tileSet.m_TileWidth+tileSet.m_Spacing);
        int pixelY = (tileSet.m_ImageHeight - (y+1))*(tileSet.m_TileHeight+tileSet.m_Spacing);
        return { pixelX / (float)tileSet.m_ImageWidth, pixelY / (float)tileSet.m_ImageHeight };
    };

    static math::Vector2f MaxTexCoord( const TiledTileSet& tileSet, int id ) {
        int x = id / (tileSet.m_ImageWidth / tileSet.m_TileWidth);
        int y = id % (tileSet.m_ImageWidth / tileSet.m_TileWidth);
        int pixelX = (x+1)*(tileSet.m_TileWidth+tileSet.m_Spacing);
        int pixelY = (tileSet.m_ImageHeight - y)*(tileSet.m_TileHeight+tileSet.m_Spacing);
        return { pixelX / (float)tileSet.m_ImageWidth, pixelY / (float)tileSet.m_ImageHeight };
    };

    void Background::Load( const TiledLevel& tiledLevel ) {

        for( int i = 0; i < tiledLevel.m_NumLayers; ++i ) {
            Bucket buckets[tiledLevel.m_NumTileSets];
            for( int j = 0; j < tiledLevel.m_Layers[i].m_NumCells; ++j ) {
                int tileSetId = FindTileSet( tiledLevel, tiledLevel.m_Layers[i].m_Data[j].m_Tile ); 
                buckets[tileSetId].m_Cells.push_back(tiledLevel.m_Layers[i].m_Data[j]);
            }
            for( int j = 0; j < tiledLevel.m_NumTileSets; ++j ) {
                std::vector<TiledCell>& cells = buckets[j].m_Cells; 
                if( cells.size() > 0 ) {
                    math::Vector2f vertices[cells.size()*4];
                    math::Vector2f texCoords[cells.size()*4];
                    unsigned short indices[cells.size()*6];
                    for( unsigned int k = 0; k < cells.size(); ++k ) {
                        vertices[k*4].m_X = cells[k].m_X; 
                        vertices[k*4].m_Y = tiledLevel.m_Height - (cells[k].m_Y+1); 
                        vertices[k*4+1].m_X = cells[k].m_X+1; 
                        vertices[k*4+1].m_Y = tiledLevel.m_Height - (cells[k].m_Y+1); 
                        vertices[k*4+2].m_X = cells[k].m_X+1; 
                        vertices[k*4+2].m_Y = tiledLevel.m_Height - cells[k].m_Y; 
                        vertices[k*4+3].m_X = cells[k].m_X; 
                        vertices[k*4+3].m_Y = tiledLevel.m_Height - cells[k].m_Y; 


                        math::Vector2f min = MinTexCoord( tiledLevel.m_TileSets[j], cells[k].m_Tile ); 
                        math::Vector2f max = MaxTexCoord( tiledLevel.m_TileSets[j], cells[k].m_Tile ); 
                        texCoords[k*4].m_X = min.m_X; 
                        texCoords[k*4].m_Y = min.m_Y;
                        texCoords[k*4+1].m_X = max.m_X; 
                        texCoords[k*4+1].m_Y = min.m_Y; 
                        texCoords[k*4+2].m_X = max.m_X; 
                        texCoords[k*4+2].m_Y = max.m_Y; 
                        texCoords[k*4+3].m_X = min.m_X; 
                        texCoords[k*4+3].m_Y = max.m_Y; 

                        indices[k*6] = k*4;
                        indices[k*6 + 1] = k*4 + 1;
                        indices[k*6 + 2] = k*4 + 3;
                        indices[k*6 + 3] = k*4 + 1;
                        indices[k*6 + 4] = k*4 + 2;
                        indices[k*6 + 5] = k*4 + 3;
                    }
                    Chunk chunk;
                    chunk.m_Vertices.AddData(vertices,cells.size()*4);
                    chunk.m_TexCoords.AddData(texCoords,cells.size()*4);
                    chunk.m_Indices.AddData(indices,cells.size()*6);
                    chunk.m_Texture = dali::textureLoader.Load(tiledLevel.m_TileSets[j].m_ImageName);
                    m_Chunks.push_back(chunk);
                }
            }
        }

    }

    void Background::Draw( double elapsedTime ) {
        for( unsigned int i = 0; i < m_Chunks.size(); ++i ) {
            const Chunk& chunk = m_Chunks[i];
            Context::m_Renderer.Draw( chunk.m_Vertices, chunk.m_TexCoords, chunk.m_Indices, *chunk.m_Texture, 0 );
        }
    }
}
