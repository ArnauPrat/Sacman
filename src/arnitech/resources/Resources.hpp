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

#ifndef ATRESOURCES_H
#define ATRESOURCES_H

#include "common/Platform.hpp"
#include "renderer/Effect.hpp"
#include "renderer/PixelShader.hpp"
#include "renderer/Sprite.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexShader.hpp"
#include "PathFinder.hpp"
#include "ResourceLoader.hpp"

typedef atResourceLoader<atTexture>       atTextureLoader; 
typedef atResourceLoader<atSprite>        atSpriteLoader; 
typedef atResourceLoader<atEffect>        atEffectLoader;
typedef atResourceLoader<atPixelShader>   atPixelShaderLoader;
typedef atResourceLoader<atVertexShader>  atVertexShaderLoader;

ATEXPORT extern atTextureLoader          textureLoader;
ATEXPORT extern atSpriteLoader           spriteLoader;
ATEXPORT extern atEffectLoader           effectLoader;
ATEXPORT extern atVertexShaderLoader     vertexShaderLoader;
ATEXPORT extern atPixelShaderLoader      pixelShaderLoader;
ATEXPORT extern atPathFinder             pathFinder;

#endif
