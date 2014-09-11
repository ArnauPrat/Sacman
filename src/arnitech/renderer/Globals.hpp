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

#ifndef ATGLOBALS_H
#define ATGLOBALS_H

#include "Effect.hpp"
#include "PixelShader.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "VertexShader.hpp"
#include "resources/PathFinder.hpp"
#include "resources/ResourceLoader.hpp"

typedef atResourceLoader<atTexture>       atTextureLoader; 
typedef atResourceLoader<atSprite>        atSpriteLoader; 
typedef atResourceLoader<atEffect>        atEffectLoader;
typedef atResourceLoader<atPixelShader>   atPixelShaderLoader;
typedef atResourceLoader<atVertexShader>  atVertexShaderLoader;

extern atTextureLoader          textureLoader;
extern atSpriteLoader           spriteLoader;
extern atEffectLoader           effectLoader;
extern atVertexShaderLoader     vertexShaderLoader;
extern atPixelShaderLoader      pixelShaderLoader;
extern atPathFinder             pathFinder;

#endif
