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

#ifndef ATBUFFER_H
#define ATBUFFER_H

#include <GL/glew.h>
#include <cassert>
#include <cfloat>
#include <math/Types.hpp>

template <class T, GLenum BufferType >
class atBuffer {
    public:
        atBuffer() :
            m_Data( GL_INVALID_VALUE ),
            m_NumElements( 0 ) {
            }

        atBuffer( const atBuffer<T, BufferType>& buffer ) 
        {
        }

        ~atBuffer() {
            if( m_Data != GL_INVALID_VALUE ) {
                glDeleteBuffers( 1, &m_Data );
            }
        }

        void AddData( const T vertices[], const int numElements ) {
            glGenBuffers(1,&m_Data);
            assert( m_Data != GL_INVALID_VALUE );
            m_NumElements = numElements;
            glBindBuffer( BufferType, m_Data );
            glBufferData( BufferType, sizeof( T )*m_NumElements, vertices, GL_STATIC_DRAW );
            glBindBuffer( BufferType, 0 );
        }

        GLuint m_Data;
        int    m_NumElements;
}; 

typedef atBuffer<atVector2f, GL_ARRAY_BUFFER> atVector2fBuffer;
typedef atBuffer<atVector2f, GL_ARRAY_BUFFER> atTexCoordBuffer;
typedef atBuffer<unsigned short, GL_ELEMENT_ARRAY_BUFFER> atIndexBuffer;

class atVertexBuffer : public atVector2fBuffer {
    public:
        void AddData(const atVector2f vertices[], const int numElements) {
            atVector2fBuffer::AddData(vertices, numElements);
            m_AABBMin = { FLT_MAX, FLT_MAX };
            m_AABBMax = { -FLT_MAX, -FLT_MAX };
            for (int i = 0; i < numElements; ++i) {
                m_AABBMin.m_X = m_AABBMin.m_X > vertices[i].m_X ? vertices[i].m_X : m_AABBMin.m_X;
                m_AABBMin.m_Y = m_AABBMin.m_Y > vertices[i].m_Y ? vertices[i].m_Y : m_AABBMin.m_Y;
                m_AABBMax.m_X = m_AABBMax.m_X < vertices[i].m_X ? vertices[i].m_X : m_AABBMax.m_X;
                m_AABBMax.m_Y = m_AABBMax.m_Y < vertices[i].m_Y ? vertices[i].m_Y : m_AABBMax.m_Y;
            }
        }
        atVector2f      m_AABBMin;
        atVector2f      m_AABBMax;
};
#endif
