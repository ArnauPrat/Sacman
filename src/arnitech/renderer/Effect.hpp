/*Dali is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Dali is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ATEFFECT_H
#define ATEFFECT_H

#include "GL/glew.h"
#include "GLSL.hpp"
#include "PixelShader.hpp"
#include "VertexShader.hpp"
#include <functional>
#include <map>

class atEffect {
    public:
        atEffect();
        ~atEffect();

        /** @brief Loads an effect 
         *  @param filename The name of the texture.*/
        void Load( const char* filename );

        /** @brief Sets a state function to set the state of a shader variable.
         *  @param var The variable to set.
         *  @param fp The pointer to the state function.*/
        static void SetStateFunction( const atGlslVar var, std::function< void (GLuint) > );

        /** @brief Sets the state of an effect*/ 
        static void SetEffect( const atEffect& effect );

    private:
        GLuint  m_ShaderID;
        GLint   m_ShaderVars[E_NUM_VARS];

        static std::function< void (GLint) > 	m_StateFunctions[E_NUM_VARS];	/**< @brief Functions used to feed shaders' variables with values.*/

};

#endif 
