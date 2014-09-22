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

#ifndef SACMAN_STATICSPRITE_H
#define SACMAN_STATICSPRITE_H

#include "common/Platform.hpp"
#include "common/Macros.hpp"
#include "Entity.hpp"
#include "renderer/Buffer.hpp"
#include "renderer/Texture.hpp"

ATENTITY_CLASS(atStaticSprite) {
    ATNON_COPYABLE(atStaticSprite)
    public:
        ATEXPORT atStaticSprite(const char* name, const char* textureName, const atVector2f texCoords[4] );
        ATEXPORT ~atStaticSprite();

        ATENTITY_ABSTRACT
        ATENTITY_DRAW
        ATENTITY_DRAW_SHAPE
        ATENTITY_ENTER
        ATENTITY_LEAVE

    private:
        static const char*              m_Type;
        atTexCoordBuffer                m_TexCoords;
        atTexture*                      m_Texture;
        atVector2f                      m_Position;
        atVector2f                      m_Extent;
};

#endif
