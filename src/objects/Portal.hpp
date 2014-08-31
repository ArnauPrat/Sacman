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

#include "Context.hpp"
#include "dali/Buffer.hpp"
#include "dali/Texture.hpp"
#include "Body.hpp"

namespace sacman {
    class Portal : public Entity {
        public:
            Portal(const char* name,
                const char* targetLevel,
                const math::Vector2f& targetPosition);
            ~Portal();

            void Draw( const double elapsedTime, const int depth ) const ;
            void DrawShape( const double elapsedTime, const int depth ) const;
            void Update( const double elapsedTime );
            void LoadTexture( const char* textureName, const math::Vector2f texCoords[4] );
            void Enter(Level* level, const math::Vector2f position, const math::Vector2f& extent);
            void Leave( Level* level );

            virtual math::Vector2f Position() const;
            virtual void SetPosition( const math::Vector2f& position );
            virtual math::Vector2f Extent() const;
            virtual const char* Type() const; 

        private:
            static const char*      m_Type;
            dali::Texture*          m_Texture;
            dali::Vector2fBuffer    m_TexCoords;
            char                    m_TargetLevel[LEVEL_NAME_LENGTH];
            math::Vector2f          m_TargetPosition;
            Body                    m_Body;

    };
}

#endif
