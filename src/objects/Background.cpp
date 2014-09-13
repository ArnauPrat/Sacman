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

void Background::InsertChunk( Chunk* chunk ) {
    m_Chunks.push_back(chunk);
    atVector2f min = chunk->m_Min;
    atVector2f max = chunk->m_Max;
    m_Min.m_X = m_Min.m_X > min.m_X ? min.m_X : m_Min.m_X;
    m_Min.m_Y = m_Min.m_Y > min.m_Y ? min.m_Y : m_Min.m_Y;
    m_Max.m_X = m_Max.m_X < max.m_X ? max.m_X : m_Max.m_X;
    m_Max.m_Y = m_Max.m_Y < max.m_Y ? max.m_Y : m_Max.m_Y;
}

#undef CHUNKSIZE
