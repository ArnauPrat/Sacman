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
#include "resources/Resources.hpp"
#include <GL/glew.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string.h>
#include <string>

std::function< void (GLint) > 	atEffect::m_StateFunctions[E_NUM_VARS];

atEffect::atEffect() :
    m_ShaderID( GL_INVALID_VALUE ){
        for( int i = 0; i < E_NUM_VARS; ++i ) {
            m_ShaderVars[i] = GL_INVALID_VALUE; 
        }
    }

atEffect::~atEffect() {
    if( m_ShaderID != GL_INVALID_VALUE ) {
        glDeleteProgram( m_ShaderID );
    }
}

void atEffect::Load( const char* filename ) {
    m_ShaderID = glCreateProgram();
    std::ifstream inFile;
    inFile.open(filename);

    std::string vertexShaderName;
    std::string pixelShaderName;

    inFile >> vertexShaderName;
    inFile >> pixelShaderName;

    atVertexShader* vertexShader = vertexShaderLoader.Load(vertexShaderName.c_str());
    atPixelShader* pixelShader = pixelShaderLoader.Load(pixelShaderName.c_str());

    glAttachShader(m_ShaderID,vertexShader->m_ShaderID);
    glAttachShader(m_ShaderID,pixelShader->m_ShaderID);
    glBindAttribLocation(m_ShaderID, 0, "aPosition");
    glBindAttribLocation(m_ShaderID, 1, "aTexCoord");
    glLinkProgram(m_ShaderID);

    GLint result;
    glGetProgramiv(m_ShaderID,GL_LINK_STATUS,&result);

    if(result==GL_FALSE) {
        int len;
        char log[256];
        glGetShaderInfoLog(m_ShaderID, 256, &len, log);
        atContext::log->Error("Error linking effect: %s ",filename);
        atContext::log->Error("%s ",log);
    }

    inFile.close();

    for( unsigned i = 0; i < vertexShader->m_Vars.size(); i++ ) {
        m_ShaderVars[vertexShader->m_Vars[i]] = glGetUniformLocation(m_ShaderID,glslVarNames[vertexShader->m_Vars[i]]);
    }

    for( unsigned i = 0; i < pixelShader->m_Vars.size(); i++ ) {
        m_ShaderVars[pixelShader->m_Vars[i]] = glGetUniformLocation(m_ShaderID,glslVarNames[pixelShader->m_Vars[i]]);
    }
}

void atEffect::SetStateFunction( const atGlslVar var, std::function< void ( GLuint ) > fp ) {
    m_StateFunctions[var] = fp;
}

void atEffect::SetEffect( const atEffect& effect ) {
    glUseProgram( effect.m_ShaderID );
    for( int i = 0; i < E_NUM_VARS; ++i ) {
        if( effect.m_ShaderVars[i] != GL_INVALID_VALUE ) {
            m_StateFunctions[i]( effect.m_ShaderVars[i] );
        }	
    }
}
