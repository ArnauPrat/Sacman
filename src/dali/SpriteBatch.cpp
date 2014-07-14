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
#include<cstring>

namespace dali {
    SpriteBatch::SpriteBatch( const Texture& texture ) :
        m_Texture( texture ),
        m_TexQuads( NULL ) {
        }

    SpriteBatch::~SpriteBatch() {
        if( m_TexQuads ) {
            delete m_TexQuads;
        }
    }

    void SpriteBatch::AddTexQuads( const TexQuad texQuads[], const int numQuads ) {
        m_TexQuads = new TexQuad[numQuads];
        std::memcpy( m_TexQuads, texQuads, sizeof(TexQuad)*numQuads ); 
    }
}
