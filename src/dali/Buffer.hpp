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

#ifndef BUFFER_H
#define BUFFER_H
#include <GL/glew.h>
#include <cassert>

namespace dali {
    template <class T, GLenum BufferType >
    class Buffer {
        friend class Renderer;
        public:

        Buffer() :
            m_Data( GL_INVALID_VALUE ),
            m_NumElements( 0 ) {
        }

        ~Buffer() {
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

        private:
            GLuint m_Data;
            int    m_NumElements;
    }; 
}
#endif
