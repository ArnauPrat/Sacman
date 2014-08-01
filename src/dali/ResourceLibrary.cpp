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

#include "ResourceLibrary.hpp"

namespace dali {

    ResourceLibrary::ResourceLibrary() {
    }

    ResourceLibrary::~ResourceLibrary() {
    }

    Texture* ResourceLibrary::LoadTexture( const char* fileName ) {
        return m_TextureLoader.Load( *this, fileName );
    }

    Effect*  ResourceLibrary::LoadEffect( const char* fileName ) {
        return m_EffectLoader.Load( *this, fileName );
    }

    VertexShader* ResourceLibrary::LoadVS( const char* fileName ) {
        return m_VSLoader.Load( *this, fileName );
    }

    PixelShader*  ResourceLibrary::LoadPS( const char* fileName ) {
        return m_PSLoader.Load( *this, fileName );
    }

    Sprite* ResourceLibrary::LoadSprite( const char* fileName ) {
        return m_SpriteLoader.Load( *this, fileName );
    }
}
