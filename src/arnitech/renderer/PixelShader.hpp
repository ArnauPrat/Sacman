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

#ifndef ATPIXELSHADER_H
#define ATPIXELSHADER_H


#include "common/Macros.hpp"
#include "GLSL.hpp"
#include <GL/glew.h>
#include <vector>

class atPixelShader {
    ATNON_COPYABLE(atPixelShader)
    friend class atEffect;
    public:
    ATEXPORT atPixelShader();
    ATEXPORT ~atPixelShader();

    /** @brief Loads a pixel shader 
     *  @param filename The name of the texture.*/
    ATEXPORT void Load( const char* filename );

    private:

    GLuint                      m_ShaderID; /**< @brief The pixel shader identifier.*/
    std::vector<atGlslVar>      m_Vars;     /**< @brief The variables found in the shader.*/
};

#endif 
