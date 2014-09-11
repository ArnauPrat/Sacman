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

#include "arnitech/scene/Entity.hpp"
#include "arnitech/renderer/Buffer.hpp"
#include "arnitech/renderer/Texture.hpp"

class StaticSprite : public atEntity {
    public:
        StaticSprite(const char* name, const char* textureName, const atVector2f texCoords[4] );
        ~StaticSprite();

        atVector2f Position() const ;
        atVector2f Extent() const ;
        void SetPosition( const atVector2f& position );
        const char* Type() const;
        void Draw( const double elapsedTime, const int depth ) const;
        void DrawShape( const double elapsedTime, const int depth ) const;
        void Enter( atLevel* level, const atVector2f position, const atVector2f& extent);
        void Leave( atLevel* level );

    private:
        static const char*              m_Type;
        atTexCoordBuffer                m_TexCoords;
        atTexture*                      m_Texture;
        atVector2f                      m_Position;
        atVector2f                      m_Extent;
};

#endif
