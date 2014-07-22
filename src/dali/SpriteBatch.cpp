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

#include "SpriteBatch.hpp"
#include <GL/glew.h>
#include <cassert>
#include <cstring>

namespace dali {
    SpriteBatch::SpriteBatch() :
        m_Vertices( GL_INVALID_VALUE ),
        m_TexCoords( GL_INVALID_VALUE ),
        m_Indices( GL_INVALID_VALUE ),
        m_NumQuads ( 0 )    {
        }

    SpriteBatch::~SpriteBatch() {
        if( m_Vertices ) {
            glDeleteBuffers(1, &m_Vertices);
        }

        if( m_TexCoords ) {
            glDeleteBuffers(1, &m_TexCoords);
        }

        if( m_Indices ) {
            glDeleteBuffers(1, &m_Indices);
        }
    }

    void SpriteBatch::AddTexQuads( const TexQuad texQuads[], const short numQuads, Texture& texture ) {
	glGenBuffers(1,&m_Vertices);
	glGenBuffers(1,&m_TexCoords);
	glGenBuffers(1,&m_Indices);
	assert( m_Vertices != GL_INVALID_VALUE );
	assert( m_TexCoords != GL_INVALID_VALUE );
	assert( m_Indices != GL_INVALID_VALUE );
    m_NumQuads = numQuads;
    m_Texture = &texture;
	float* tempBuffer = new float[m_NumQuads*4*2];	
	int j = 0;
	for( int i = 0; i < m_NumQuads; ++i ) {
		tempBuffer[j++] = texQuads[i].m_Min.m_X;
		tempBuffer[j++] = texQuads[i].m_Min.m_Y;

		tempBuffer[j++] = texQuads[i].m_Max.m_X;
		tempBuffer[j++] = texQuads[i].m_Min.m_Y;

		tempBuffer[j++] = texQuads[i].m_Max.m_X;
		tempBuffer[j++] = texQuads[i].m_Max.m_Y;

		tempBuffer[j++] = texQuads[i].m_Min.m_X;
		tempBuffer[j++] = texQuads[i].m_Max.m_Y;
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_Vertices );
	glBufferData( GL_ARRAY_BUFFER, sizeof( float )*m_NumQuads*4*2, tempBuffer, GL_STATIC_DRAW );

	j = 0;
	for( int i = 0; i < m_NumQuads; ++i ) {
		tempBuffer[j++] = texQuads[i].m_TexMin.m_X;
		tempBuffer[j++] = texQuads[i].m_TexMin.m_Y;

		tempBuffer[j++] = texQuads[i].m_TexMax.m_X;
		tempBuffer[j++] = texQuads[i].m_TexMin.m_Y;

		tempBuffer[j++] = texQuads[i].m_TexMax.m_X;
		tempBuffer[j++] = texQuads[i].m_TexMax.m_Y;

		tempBuffer[j++] = texQuads[i].m_TexMin.m_X;
		tempBuffer[j++] = texQuads[i].m_TexMax.m_Y;
	}

	glBindBuffer( GL_ARRAY_BUFFER, m_TexCoords );
	glBufferData( GL_ARRAY_BUFFER, sizeof( float )*m_NumQuads*4*2, tempBuffer, GL_STATIC_DRAW );
	delete [] tempBuffer;
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	unsigned short* indexBuffer = new unsigned short[m_NumQuads*6];
	j = 0;
	for( int i = 0; i < m_NumQuads; ++i ) {
		indexBuffer[j++] = i*4;
		indexBuffer[j++] = i*4 + 1;
		indexBuffer[j++] = i*4 + 3;

		indexBuffer[j++] = i*4 + 1;
		indexBuffer[j++] = i*4 + 2;
		indexBuffer[j++] = i*4 + 3;
	}
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Indices );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short )*m_NumQuads*6, indexBuffer, GL_STATIC_DRAW );
	delete [] indexBuffer;
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
}
