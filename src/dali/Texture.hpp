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

namespace dali {
    class ResourceLibrary;
    class Texture {
	    friend class Renderer;
        public:
            ~Texture();

            /** @brief Loads a texture
             *  @param library The resource library used to load other resources.
             *  @param filename The name of the texture.*/
            void Load( ResourceLibrary& library, const char* filename );

        private:
            template<class T> friend class ResourceLoader;
            Texture();
            GLuint m_TextureID; /**< @brief The texture identifier*/
    };

}

#endif
