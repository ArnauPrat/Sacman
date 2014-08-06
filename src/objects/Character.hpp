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

#ifndef CHARACTER_H
#define CHARACTER_H

#include "dali/SpriteRenderer.hpp"
#include "math/Types.hpp"
#include "dali/Dali.hpp"
#include <Box2D/Box2D.h>


namespace sacman {
    class Level;
    class Character {
        public:
            Character( Level& level );
            ~Character();

            /** @brief Loads a character using a given sprite.
             *  @param spriteName The name of the sprite.*/
            void Load( const char* spriteName );

            /** @brief Draws the sprite.
             *  @param elapsedTime The elapsed time since the last call to draw.*/
            void Draw( const double elapsedTime, const int depth ) const ;

            void DrawShape() const;

            /** @brief Launches an animation.
             *  @param name The name of the animation.
             *  @param loop True if we want to loop.*/
            void LaunchAnimation( const char* name, const double totalTime, const bool loop  );

            /** @brief Stops the current animation.*/
            void StopAnimation();

            /** @brief Tells if the whe given animation is running.
             *  @return True if it is running. False otherwise.*/
            bool IsAnimationRunning( const char* name );


            math::Vector2f        m_Position;
            math::Vector2f        m_Scale;

        private:
            dali::SpriteRenderer*   m_SpriteRenderer;
            Level&                  m_Level;
            b2Body*                 m_Body;
            b2Fixture*              m_Fixture;
    };
}


#endif
