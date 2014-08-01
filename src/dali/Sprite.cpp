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

#include "ResourceLibrary.hpp"
#include "Sprite.hpp"
#include "Renderer.hpp"

namespace dali {

    Sprite::Sprite() {

    }

    Sprite::~Sprite() {

    }

    void Sprite::Load( ResourceLibrary& library, const char* fileName ) {
        m_Texture = library.LoadTexture( "./textures/ball.png");
        dali::Vector2f data[4];
        data[0].m_X = 0.0f;
        data[0].m_Y = 0.0f;
        data[1].m_X = 1.0f;
        data[1].m_Y = 0.0f;
        data[2].m_X = 1.0f;
        data[2].m_Y = 1.0f;
        data[3].m_X = 0.0f;
        data[3].m_Y = 1.0f;
        m_Vertices.AddData( data, 4 );
        m_TexCoords.AddData( data, 4 );
        unsigned short indexData[6];
        indexData[0] = 0;
        indexData[1] = 1;
        indexData[2] = 3;
        indexData[3] = 1;
        indexData[4] = 2;
        indexData[5] = 3;
        m_Indices.AddData( indexData, 6 );
    }

    void Sprite::Draw( Renderer& renderer, const dali::Vector2f& position, 
                       const dali::Vector2f& scale ) {
        renderer.Draw( m_Vertices, m_TexCoords, m_Indices, *m_Texture, position, scale );
    }
}
