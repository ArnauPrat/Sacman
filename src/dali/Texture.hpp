/*Dali is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Dali is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

namespace dali {

    class Texture {
        public:
            ~Texture();

            /** @brief Loads a texture
             *  @param filename The name of the texture.*/
            void Load( const std::string& filename );

        private:
            template<class T> friend class ResourceLoader;
            Texture();
            GLuint m_TextureID; /**< @brief The texture identifier*/
    };

}

#endif
