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

#ifndef ATSPRITERENDERER_H
#define ATSPRITERENDERER_H

#include "common/Macros.hpp"
#include "Sprite.hpp"

class atSpriteRenderer {
    ATNON_COPYABLE(atSpriteRenderer)
    public:
        atSpriteRenderer( const atSprite& sprite );
        ~atSpriteRenderer();

        /** @brief Draws the sprite.
         *  @param renderer The renderer to use.
         *  @param elapsedTime The elapsed time since the last call to draw.
         *  @param position The position to write the sprite to.
         *  @param scale The scale of the sprite.*/
        void Draw( atRenderer& renderer, 
                const double elapsedTime,
                const int depth,
                const atVector2f& position = {0.0f, 0.0f}, 
                const atVector2f& scale = {2.0f, 2.0f} );

        /** @brief Launches an animation.
         *  @param name The name of the animation.
         *  @param loop True if we want to loop.*/
        void LaunchAnimation( const char* name, double totalTime, bool loop  );

        /** @brief Stops the current animation.*/
        void StopAnimation();

        /** @brief Tells if the whe given animation is running.
         *  @return True if it is running. False otherwise.*/
        bool IsAnimationRunning( const char* name );

    private:
        /** @brief Gets the index of an animation.
         *  @param name The name of the animation.
         *  @return The index.*/
        char GetAnimationIndex( const char* name );

        const atSprite&     m_Sprite;
        float               m_TotalTime;
        float               m_ElapsedTime;
        bool                m_AnimationRunning;
        char                m_AnimationIndex;
        bool                m_Loop;
};
#endif
