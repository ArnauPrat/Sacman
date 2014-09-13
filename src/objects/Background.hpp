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

#ifndef SACMANBACKGROUND_HPP 
#define SACMANBACKGROUND_HPP 

#include "renderer/Buffer.hpp"
#include "renderer/Texture.hpp"
#include "scene/Entity.hpp"
#include <vector>

struct atTiledLevel;
struct atTiledTileSet;
struct atTiledCell;
struct atTiledLayer;

struct Chunk {
    atVertexBuffer      m_Vertices;
    atTexCoordBuffer    m_TexCoords;
    atIndexBuffer       m_Indices;
    atTexture*          m_Texture;
    int                 m_Depth;
    atVector2f          m_Min;
    atVector2f          m_Max;
};

class Background : public atEntity {

    public:
    Background( const char* name );
    ~Background();

    void                    Load( const atTiledLevel& tiledLevel );
    void                    Draw( const double elapsedTime, const int depth ) const;
    void                    DrawShape(const double elapsedTime, const int depth) const;
    atVector2f              Position() const;
    void                    SetPosition( const atVector2f& position );
    atVector2f              Extent() const;
    const char*             Type() const;
    
    void                    InsertChunk( Chunk* chunk );

    private:

    static const char*      m_Type;
    std::vector<Chunk*>     m_Chunks;
    atVector2f              m_Min;
    atVector2f              m_Max;
};
#endif
