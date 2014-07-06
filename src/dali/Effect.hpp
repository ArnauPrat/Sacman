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

#ifndef EFFECT_H
#define EFFECT_H

#include "GL/glew.h"
#include "GLSL.hpp"
#include "PixelShader.hpp"
#include "ResourceLoader.hpp"
#include "VertexShader.hpp"
#include <map>

namespace dali {
    class Effect {
        public:
            ~Effect();

            /** @brief Loads an effect 
             *  @param filename The name of the texture.*/
            void Load( const std::string& filename );
        private:
            template<class T> friend class ResourceLoader;
            Effect();

            GLuint m_ShaderID;
            std::map<GlslVar,GLuint> m_ShaderVars;

            static ResourceLoader<VertexShader> m_VertexShaderLoader;   /**< @brief The loader of pixel shaders.*/
            static ResourceLoader<PixelShader> m_PixelShaderLoader;     /**< @brief The loader of vertex shaders.*/

    };
}


#endif 
