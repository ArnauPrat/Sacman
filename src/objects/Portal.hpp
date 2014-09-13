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

#ifndef SACMAN_PORTAL_H
#define SACMAN_PORTAL_H

#include "arnitech/system/Context.hpp"
#include "arnitech/renderer/Buffer.hpp"
#include "arnitech/renderer/Texture.hpp"
#include "arnitech/scene/Body.hpp"

class Portal : public atEntity {
    public:
        Portal(const char* name,
                const char* targetLevel,
                const atVector2f& targetPosition);
        ~Portal();

        void Draw( const double elapsedTime, const int depth ) const ;
        void DrawShape( const double elapsedTime, const int depth ) const;
        void Update( const double elapsedTime );
        void LoadTexture( const char* textureName, const atVector2f texCoords[4] );
        void Enter( atLevel* level, const atVector2f position, const atVector2f& extent);
        void Leave( atLevel* level );

        atVector2f Position() const;
        void SetPosition( const atVector2f& position );
        atVector2f Extent() const;
        const char* Type() const; 

    private:
        static const char*      m_Type;
        atTexture*              m_Texture;
        atVector2fBuffer        m_TexCoords;
        char                    m_TargetLevel[ATLEVEL_NAME_LENGTH];
        atVector2f              m_TargetPosition;
        atBody                  m_Body;

};
#endif
