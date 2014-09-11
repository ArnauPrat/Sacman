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
#include "arnitech/system/Context.hpp"
#include "arnitech/resources/Tiled.hpp"
#include "arnitech/renderer/Globals.hpp"
#include "arnitech/math/Types.hpp"
#include <cassert>
#include <cfloat>
#include <vector>


#define CHUNKSIZE 8 

    const char* Background::m_Type="background";

    Background::Background( const char* name ) :
    atEntity( name ) {
        m_Min = {FLT_MAX, FLT_MAX};
        m_Max = {-FLT_MAX, -FLT_MAX};
    }

    Background::~Background() {
        for( unsigned int i = 0; i < m_Chunks.size(); ++i ) {
            delete m_Chunks[i];
        }
    }

    struct Bucket {
        std::vector<atTiledCell> m_Cells;
    };

    void Background::Subdivide(const atTiledLevel& tiledLevel, const atTiledLayer& layer, const atTiledTileSet& tileSet, const atTiledCell cells[], const int numCells) {
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
                LoadChunk(tiledLevel, layer, tileSet, chunks[i].data(), static_cast<int>(chunks[i].size()));
            }
        }
        delete[] chunks;
    }

    void Background::LoadChunk(const atTiledLevel& tiledLevel, const atTiledLayer& layer, const atTiledTileSet& tileSet, const atTiledCell cells[], const int numCells) {
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
        m_Chunks.push_back(chunk);
        m_Min.m_X = m_Min.m_X > min.m_X ? min.m_X : m_Min.m_X;
        m_Min.m_Y = m_Min.m_Y > min.m_Y ? min.m_Y : m_Min.m_Y;
        m_Max.m_X = m_Max.m_X < max.m_X ? max.m_X : m_Max.m_X;
        m_Max.m_Y = m_Max.m_Y < max.m_Y ? max.m_Y : m_Max.m_Y;
        delete[] vertices;
        delete[] texCoords;
        delete[] indices;
    }

    void Background::LoadLayer(const atTiledLevel& tiledLevel, const atTiledLayer& layer) {
        Bucket* buckets = new Bucket[tiledLevel.m_NumTileSets];
        for (int j = 0; j < layer.m_TileLayer.m_NumCells; ++j) {
            int tileSetId = FindTileSet(tiledLevel, layer.m_TileLayer.m_Cells[j].m_Tile);
            buckets[tileSetId].m_Cells.push_back(layer.m_TileLayer.m_Cells[j]);
        }
        for (int j = 0; j < tiledLevel.m_NumTileSets; ++j) {
            std::vector<atTiledCell>& cells = buckets[j].m_Cells;
            if (cells.size() > 0) {
                Subdivide(tiledLevel, layer, tiledLevel.m_TileSets[j], cells.data(), static_cast<int>(cells.size()));
            }
        }
        delete[] buckets;
    }

    void Background::Load(const atTiledLevel& tiledLevel) {
        for (int i = 0; i < tiledLevel.m_NumLayers; ++i) {
            if (tiledLevel.m_Layers[i].m_Type == E_TILE_LAYER) {
                LoadLayer(tiledLevel, tiledLevel.m_Layers[i]);
            }
        }
    }

    void Background::Draw(const double elapsedTime, const int depth) const {
        for( unsigned int i = 0; i < m_Chunks.size(); ++i ) {
            const Chunk* chunk = m_Chunks[i];
            atContext::m_Renderer.Draw( &chunk->m_Vertices, &chunk->m_TexCoords, &chunk->m_Indices, chunk->m_Texture, chunk->m_Depth, static_cast<void*>(0), {0.0f, 0.0f}, {1.0f, 1.0f} );
        }
    }

    void Background::DrawShape(const double elapsedTime, const int depth) const {
        for( unsigned int i = 0; i < m_Chunks.size(); ++i ) {
            const Chunk* chunk = m_Chunks[i];
            atContext::m_Renderer.DrawBox(chunk->m_Min, { chunk->m_Max.m_X - chunk->m_Min.m_X, chunk->m_Max.m_Y - chunk->m_Min.m_Y }, { 0.0f, 1.0f, 0.0f, 1.0f }, depth);
        }
    }

    atVector2f Background::Position() const {
        return {(m_Max.m_X - m_Min.m_X) / 2.0f + m_Min.m_X, (m_Max.m_Y - m_Min.m_Y) / 2.0f + m_Min.m_Y};
    }

    void Background::SetPosition( const atVector2f& position ) {
    }

    atVector2f Background::Extent() const {
        return {(m_Max.m_X - m_Min.m_X) / 2.0f, (m_Max.m_Y - m_Min.m_Y) / 2.0f};
    }

    const char* Background::Type() const {
        return m_Type;
    }

#undef CHUNKSIZE
