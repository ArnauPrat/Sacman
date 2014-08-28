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

#include "GLSL.hpp"
#include "PixelShader.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <string.h>

namespace dali {

    PixelShader::PixelShader() :
    m_ShaderID( GL_INVALID_VALUE ) {
    }

    PixelShader::~PixelShader() {
        if( m_ShaderID != GL_INVALID_VALUE ) {
            glDeleteShader( m_ShaderID );
        }
    }

    void PixelShader::Load( const char* filename ) {

			std::ifstream inFile;
			inFile.open(filename);
			if(!inFile.is_open()) return;
			
			m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			char buffer[2048];
			std::list<std::string> source;
			while(!inFile.eof()) {
				inFile.getline(buffer,2048);
				std::string s(buffer);
				source.push_back(s);
			}
			inFile.close();
			
			char ** src = new char*[source.size()];
			int *sizes = new int[source.size()];
			
			std::list<std::string>::iterator it;
			int i =0;
			for(it=source.begin();it!=source.end();it++) {
				std::string saux = (std::string)*it;
				sizes[i] = static_cast<int>(saux.size());
				src[i] = new char[sizes[i]+1];
				strcpy(src[i],saux.c_str());
				i++;
			}
			
			glShaderSource(m_ShaderID,static_cast<GLsizei>(source.size()),(const GLchar**)src,sizes);
			glCompileShader(m_ShaderID);
			
			GLint result;
			glGetShaderiv(m_ShaderID,GL_COMPILE_STATUS,&result);
			
			int len;
			char log[256];
			glGetShaderInfoLog(m_ShaderID, 256, &len, log);
			
            if( result != GL_TRUE ) {
                std::cout << "Error loading pixel shader: " << filename << std::endl;
                std::cout << std::string(log) << std::endl;
            }

			m_Vars = ParseVars(static_cast<int>(source.size()), src, sizes); 
			
			for(int i = 0; i < source.size();i++)
			{
				delete [] src[i];
			}
			delete [] src;
			delete [] sizes;
			return;
    }
    
}
