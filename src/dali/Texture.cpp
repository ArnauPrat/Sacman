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

#include "Texture.hpp"
#include <FreeImage.h>
#include <cassert>
//#include <SOIL.h>
#include <iostream>

namespace dali {

    Texture::Texture() :
        m_TextureID( GL_INVALID_VALUE ) {
    }

    Texture::~Texture() {
        if( m_TextureID != GL_INVALID_VALUE ) {
            glDeleteTextures( 1, &m_TextureID );
        }
    }

    void Texture::Load( const char* filename ) {
/*			int width, height, channels;
			unsigned char* image = SOIL_load_image( filename, &width, &height, &channels, SOIL_LOAD_AUTO );
            assert( image != NULL );
			for( int j = 0; j*2 < height; ++j ) {
				int index1 = j * width * channels;
				int index2 = (height - 1 - j) * width * channels;
				for( int i = width * channels; i > 0; --i ) {
					unsigned char temp = image[index1];
					image[index1] = image[index2];
					image[index2] = temp;
					++index1;
					++index2;
				}
			}

			glGenTextures(1,&m_TextureID);
            if( m_TextureID != GL_INVALID_VALUE ) {
                glBindTexture(GL_TEXTURE_2D, m_TextureID);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            } else {
                std::cout << "DALI: Error loading texture " << filename << std::endl;
            }
            */
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename,0);//Automatocally detects the format(from over 20 formats!)
        FIBITMAP* image = FreeImage_Load(format, filename);

        FIBITMAP* temp = image;
        image = FreeImage_ConvertTo32Bits(image);
        FreeImage_Unload(temp);

        int w = FreeImage_GetWidth(image);
        int h = FreeImage_GetHeight(image);

        GLubyte* texture = new GLubyte[4*w*h];
        char* pixels = (char*)FreeImage_GetBits(image);

        for(int j= 0; j<w*h; j++){
            texture[j*4+0]= pixels[j*4+2];
            texture[j*4+1]= pixels[j*4+1];
            texture[j*4+2]= pixels[j*4+0];
            texture[j*4+3]= pixels[j*4+3];
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)texture );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        GLenum huboError = glGetError();
        if(huboError){
            std::cout<<"There was an error loading the texture"<<std::endl;
        }
    }
}
