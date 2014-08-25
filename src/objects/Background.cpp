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
#include <cfloat>
#include <vector>

namespace sacman {

    const char* Background::m_Type="background";

    Background::Background( const char* name ) :
    Entity( name ) {
        m_Min = {FLT_MAX, FLT_MAX};
        m_Max = {-FLT_MAX, -FLT_MAX};
    }

    Background::~Background() {
        for( unsigned int i = 0; i < m_Chunks.size(); ++i ) {
            delete m_Chunks[i];
        }
    }

    struct Bucket {
        std::vector<TiledCell> m_Cells;
    };

    void Background::Load( const TiledLevel& tiledLevel ) {
        for( int i = 0; i < tiledLevel.m_NumLayers; ++i ) {
            if( tiledLevel.m_Layers[i].m_Type == TILE_LAYER ) {
                Bucket* buckets = new Bucket[tiledLevel.m_NumTileSets];
                TiledLayer& layer = tiledLevel.m_Layers[i];
                for( int j = 0; j < layer.m_TileLayer.m_NumCells; ++j ) {
                    int tileSetId = FindTileSet( tiledLevel, layer.m_TileLayer.m_Cells[j].m_Tile ); 
                    buckets[tileSetId].m_Cells.push_back(layer.m_TileLayer.m_Cells[j]);
                }
                for( int j = 0; j < tiledLevel.m_NumTileSets; ++j ) {
                    std::vector<TiledCell>& cells = buckets[j].m_Cells; 
                    if( cells.size() > 0 ) {
                        math::Vector2f min = {FLT_MAX, FLT_MAX};
                        math::Vector2f max = {-FLT_MAX, -FLT_MAX};
                        math::Vector2f* vertices = new math::Vector2f[cells.size()*4];
                        math::Vector2f* texCoords = new math::Vector2f[cells.size()*4];
                        unsigned short* indices = new unsigned short[cells.size()*6];
                        for( unsigned int k = 0; k < cells.size(); ++k ) {
                            vertices[k*4].m_X = cells[k].m_X; 
                            vertices[k*4].m_Y = tiledLevel.m_Height - (cells[k].m_Y+1); 
                            min.m_X = min.m_X > vertices[k*4].m_X ? vertices[k*4].m_X : min.m_X;
                            min.m_Y = min.m_Y > vertices[k*4].m_Y ? vertices[k*4].m_Y : min.m_Y;
                            vertices[k*4+1].m_X = cells[k].m_X+1; 
                            vertices[k*4+1].m_Y = tiledLevel.m_Height - (cells[k].m_Y+1); 
                            vertices[k*4+2].m_X = cells[k].m_X+1; 
                            vertices[k*4+2].m_Y = tiledLevel.m_Height - cells[k].m_Y; 
                            max.m_X = max.m_X < vertices[k*4].m_X ? vertices[k*4].m_X : max.m_X;
                            max.m_Y = max.m_Y < vertices[k*4].m_Y ? vertices[k*4].m_Y : max.m_Y;
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

                        Chunk* chunk = new Chunk();
                        chunk->m_Vertices.AddData(vertices,cells.size()*4);
                        chunk->m_TexCoords.AddData(texCoords,cells.size()*4);
                        chunk->m_Indices.AddData(indices,cells.size()*6);
                        chunk->m_Texture = dali::textureLoader.Load(tiledLevel.m_TileSets[j].m_ImageName);
                        chunk->m_Depth = i;
                        chunk->m_Min = min;
                        chunk->m_Max = max;
                        m_Chunks.push_back(chunk);
                        m_Min.m_X = m_Min.m_X > min.m_X ? min.m_X : m_Min.m_X;
                        m_Min.m_Y = m_Min.m_Y > min.m_Y ? min.m_Y : m_Min.m_Y;
                        m_Max.m_X = m_Max.m_X < max.m_X ? max.m_X : m_Max.m_X;
                        m_Max.m_Y = m_Max.m_Y < max.m_Y ? max.m_Y : m_Max.m_Y;
                        delete [] vertices;
                        delete [] texCoords;
                        delete [] indices;
                    }
                }
                delete [] buckets;
            }
        }
    }

    void Background::Draw( const double elapsedTime, const int depth ) {
        for( unsigned int i = 0; i < m_Chunks.size(); ++i ) {
            const Chunk* chunk = m_Chunks[i];
            Context::m_Renderer.Draw( &chunk->m_Vertices, &chunk->m_TexCoords, &chunk->m_Indices, chunk->m_Texture, chunk->m_Depth, static_cast<void*>(0), {0.0f, 0.0f}, {1.0f, 1.0f} );
        }
    }

    math::Vector2f Background::Position() const {
        return {(m_Max.m_X - m_Min.m_X) / 2.0f + m_Min.m_X, (m_Max.m_Y - m_Min.m_Y) / 2.0f + m_Min.m_Y};
    }

    void Background::SetPosition( const math::Vector2f& position ) {
    }

    math::Vector2f Background::Extent() const {
        return {(m_Max.m_X - m_Min.m_X) / 2.0f, (m_Max.m_Y - m_Min.m_Y) / 2.0f};
    }

    const char* Background::Type() const {
        return m_Type;
    }

}
