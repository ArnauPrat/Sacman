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

#include "Effect.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string.h>
#include <string>

namespace dali {

    Effect::Effect() :
        m_ShaderID( GL_INVALID_VALUE ){
        }

    Effect::~Effect() {
        if( m_ShaderID != GL_INVALID_VALUE ) {
            glDeleteProgram( m_ShaderID );
        }
    }

    void Effect::Load( const std::string& filename ) {
        m_ShaderID = glCreateProgram();
        std::ifstream inFile;
        inFile.open(filename);

        std::string vertexShaderName;
        std::string pixelShaderName;

        inFile >> vertexShaderName;
        inFile >> pixelShaderName;

        PixelShader* pixelShader= m_PixelShaderLoader.Load(pixelShaderName.c_str());
        VertexShader* vertexShader= m_VertexShaderLoader.Load(vertexShaderName.c_str());

        glAttachShader(m_ShaderID,vertexShader->m_ShaderID);
        glAttachShader(m_ShaderID,pixelShader->m_ShaderID);
        glLinkProgram(m_ShaderID);

        GLint result;
        glGetProgramiv(m_ShaderID,GL_LINK_STATUS,&result);

        if(result==GL_FALSE) {
            int len;
            char log[256];
            glGetShaderInfoLog(m_ShaderID, 256, &len, log);
            std::cout << "Error loading effect: " << filename << std::endl;
            std::cout << std::string(log) << std::endl;
        }

        inFile.close();

        for( uint32_t i = 0; i < vertexShader->m_Vars.size(); i++ ) {
            auto it = m_ShaderVars.find(vertexShader->m_Vars[i]);
            if( it == m_ShaderVars.end() ) {
                m_ShaderVars.insert(std::pair<GlslVar, GLuint>(vertexShader->m_Vars[i], glGetUniformLocation(m_ShaderID,glslVarNames[vertexShader->m_Vars[i]])));
            }
        }

        for( uint32_t i = 0; i < pixelShader->m_Vars.size(); i++ ) {
            auto it = m_ShaderVars.find(pixelShader->m_Vars[i]);
            if( it == m_ShaderVars.end() ) {
                m_ShaderVars.insert(std::pair<GlslVar, GLuint>(pixelShader->m_Vars[i], glGetUniformLocation(m_ShaderID,glslVarNames[pixelShader->m_Vars[i]])));
            }
        }
    }
}
