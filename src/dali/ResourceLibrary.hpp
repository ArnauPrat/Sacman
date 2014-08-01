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

#ifndef RESOURCE_LIBRARY_H
#define RESOURCE_LIBRARY_H

#include "Effect.hpp"
#include "PixelShader.hpp"
#include "ResourceLoader.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Types.hpp"
#include "VertexShader.hpp"

namespace dali {

    typedef ResourceLoader<Texture> TextureLoader;
    typedef ResourceLoader<Effect> EffectLoader;
    typedef ResourceLoader<VertexShader> VSLoader;
    typedef ResourceLoader<PixelShader> PSLoader;
    typedef ResourceLoader<Sprite> SpriteLoader;

    class ResourceLibrary {
        public:

            ResourceLibrary();
            ~ResourceLibrary();

        /** @brief Loads a texture.
         *  @param fileName The path to the texture file.
         *  @return A pointer to the texture */
        Texture* LoadTexture( const char* fileName );

        /** @brief Loads an effect.
         *  @param fileName The path to the effect file .
         *  @return A pointer to the effect */
        Effect* LoadEffect( const char* fileName );

        /** @brief Loads a vertex shader.
         *  @param fileName The path to the vertex shader file .
         *  @return A pointer to the vertex shader */
        VertexShader* LoadVS( const char* fileName );

        /** @brief Loads a pixel shader.
         *  @param fileName The path to the pixel shader file .
         *  @return A pointer to the pixel shader */
        PixelShader*  LoadPS( const char* fileName );

        /** @brief Loads a sprite.
         *  @param fileName The path to the sprite file .
         *  @return A pointer to the sprite */
        Sprite* LoadSprite( const char* fileName );

        private:
        ResourceLibrary( const ResourceLibrary& );

        VSLoader                m_VSLoader;
        PSLoader                m_PSLoader;
		EffectLoader        	m_EffectLoader;     /**< @brief The effect resource loader.*/
		TextureLoader        	m_TextureLoader;    /**< @brief The texture resource loader.*/
        SpriteLoader            m_SpriteLoader;     /**< @brief The sprite resource loader.*/
    };

}

#endif
