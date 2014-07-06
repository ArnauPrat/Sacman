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

#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "GLSL.hpp"
#include <GL/glew.h>
#include <string>
#include <vector>

namespace dali {
    class VertexShader {
        public:
            ~VertexShader();

            /** @brief Loads a vertex shader 
             *  @param filename The name of the texture.*/
            void Load( const std::string& filename );
        private:
            template<class T> friend class ResourceLoader;
            friend class Effect;
            VertexShader();

            GLuint  m_ShaderID;          /**< @brief The pixel shader identifier.*/
            std::vector<GlslVar> m_Vars; /**< @brief The variables found in the shader.*/
    };
}


#endif 
