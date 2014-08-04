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
#include <Box2D/Box2D.h>

namespace sacman {
    class Level;
    class Object {
        public:
            Object( Level& level, const math::Vector2f& position, const math::Vector2f& scale );
            virtual ~Object();

            /** @brief Loads the texture of the object.
             *  @param spriteName The name of the sprite.*/
            void LoadTexture( const char* textureName, const math::Vector2f texCoords[4] );

            /** @brief Draws the object if an sprite is attached.
             *  @param elapsedTime The elapsed time since the last call to draw.*/
            void Draw( const int depth ) const ;

            void DrawShape() const;

            /** @brief Sets a box as a shape.*/
            void SetBoxShape();

            /** @brief Sets a polygon as a shape.
             *  @param vertexes The array of vertexes that form the polygon. They are treated as a loop.
             *  @param numVertexes The number of vertexes in the polygon.*/
            void SetPolyShape( const math::Vector2f vertexes[], const int numVertexes ); 

        private:
            Object( const Object& o );
            Object& operator=( const Object& o );

            Level&                  m_Level;

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

