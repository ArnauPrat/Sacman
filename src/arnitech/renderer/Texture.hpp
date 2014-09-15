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

#ifndef ATTEXTURE_H
#define ATTEXTURE_H

#include "common/Macros.hpp"
#include <GL/glew.h>

class atTexture {
    ATNON_COPYABLE(atTexture)
    friend class atRenderer;
    public:
    atTexture();
    ~atTexture();

    /** @brief Loads a texture
     *  @param library The resource library used to load other resources.
     *  @param filename The name of the texture.*/
    void Load( const char* filename );

    private:
    GLuint m_TextureID; /**< @brief The texture identifier*/
};

#endif
