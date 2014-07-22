/*Dali is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Dali is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#include "VertexBuffer.hpp"

namespace dali {

    VertexBuffer::VertexBuffer() :
    m_Vertices( GL_INVALID_VALUE ),
    m_NumVertices( 0 ) {
    }

    VertexBuffer::~VertexBuffer() {
        if( m_Vertices != GL_INVALID_VALUE ) {
            glDeleteBuffers(1, m_Vertices );
        }
    }

    void VertexBuffer::AddVertices( const Vector2f texCoords[], const int numTexCoords ) {
        glGenBuffers(1,&m_Vertices);
        assert( m_Vertices != GL_INVALID_VALUE );
        m_NumVertices = numTexCoords;
        float* tempBuffer = new float[m_NumVertices*2];	
        int j = 0;
        for( int i = 0; i < m_NumVertices; ++i ) {
            tempBuffer[j++] = texCoords[i].m_X;
            tempBuffer[j++] = texCoords[i].m_Y;
        }
        glBindBuffer( GL_ARRAY_BUFFER, m_Vertices );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float )*m_NumVertices*2, tempBuffer, GL_STATIC_DRAW );
        delete [] tempBuffer;
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
}
