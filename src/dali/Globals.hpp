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

#ifndef DALI_GLOBALS_H
#define DALI_GLOBALS_H

#include "ResourceLoader.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"
#include "PathFinder.hpp"
#include "Effect.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

namespace dali {
   typedef ResourceLoader<Texture> TextureLoader; 
   typedef ResourceLoader<Sprite> SpriteLoader; 
   typedef ResourceLoader<Effect> EffectLoader;
   typedef ResourceLoader<PixelShader> PixelShaderLoader;
   typedef ResourceLoader<VertexShader> VertexShaderLoader;
   extern TextureLoader textureLoader;
   extern SpriteLoader spriteLoader;
   extern EffectLoader effectLoader;
   extern VertexShaderLoader vertexShaderLoader;
   extern PixelShaderLoader pixelShaderLoader;
   extern PathFinder   pathFinder;
}

#endif
