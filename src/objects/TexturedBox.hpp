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

#ifndef SACMAN_OBJECT_H
#define SACMAN_OBJECT_H

#include "dali/SpriteRenderer.hpp"
#include "dali/Texture.hpp"
#include "dali/Buffer.hpp"
#include "math/Types.hpp"
#include "objects/Entity.hpp"
#include <Box2D/Box2D.h>

namespace sacman {
    class Level;
    class TexturedBox : public Entity {
        public:
            TexturedBox( const char* name, Level& level, const math::Vector2f& position, const math::Vector2f& scale );
            virtual ~TexturedBox();

            void Draw( const double elapsedTime, const int depth ) const ;

            void DrawShape( const double elapsedTime, const int depth ) const;

            /** @brief Sets a box as a shape.*/
            void SetBoxShape();

            /** @brief Loads the texture of the object.
             *  @param spriteName The name of the sprite.*/
            void LoadTexture( const char* textureName, const math::Vector2f texCoords[4] );

            virtual math::Vector2f Position() const;
            virtual void SetPosition( const math::Vector2f& position );
            virtual math::Vector2f Extent() const;
            virtual void SetExtent( const math::Vector2f& extent );

        private:
            TexturedBox( const TexturedBox& o );
            TexturedBox& operator=( const TexturedBox& o );

            math::Vector2f          m_Position;
            math::Vector2f          m_Scale;

            /** Rendering **/
            dali::Vector2fBuffer    m_TexCoords;
            dali::Texture*          m_Texture;

            /** Physics **/
            b2Body*                 m_Body;
            b2Fixture*              m_Fixture;
    };
}
#endif

