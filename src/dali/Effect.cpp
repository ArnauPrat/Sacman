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
#include "ResourceLibrary.hpp"
#include <GL/glew.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string.h>
#include <string>

namespace dali {


	std::function< void (GLint) > 	Effect::m_StateFunctions[E_NUM_VARS];

	Effect::Effect() :
		m_ShaderID( GL_INVALID_VALUE ){
			for( int i = 0; i < E_NUM_VARS; ++i ) {
				m_ShaderVars[i] = GL_INVALID_VALUE; 
			}
		}

	Effect::~Effect() {
		if( m_ShaderID != GL_INVALID_VALUE ) {
			glDeleteProgram( m_ShaderID );
		}
	}

	void Effect::Load( ResourceLibrary& library, const char* filename ) {
		m_ShaderID = glCreateProgram();
		std::ifstream inFile;
		inFile.open(filename);

		std::string vertexShaderName;
		std::string pixelShaderName;

		inFile >> vertexShaderName;
		inFile >> pixelShaderName;

		VertexShader* vertexShader= library.LoadVS(vertexShaderName.c_str());
		PixelShader* pixelShader= library.LoadPS(pixelShaderName.c_str());

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

		for( unsigned i = 0; i < vertexShader->m_Vars.size(); i++ ) {
			m_ShaderVars[vertexShader->m_Vars[i]] = glGetUniformLocation(m_ShaderID,glslVarNames[vertexShader->m_Vars[i]]);
		}

		for( unsigned i = 0; i < pixelShader->m_Vars.size(); i++ ) {
			m_ShaderVars[pixelShader->m_Vars[i]] = glGetUniformLocation(m_ShaderID,glslVarNames[pixelShader->m_Vars[i]]);
		}
	}

	void Effect::SetStateFunction( const GlslVar var, std::function< void ( GLuint ) > fp ) {
		m_StateFunctions[var] = fp;
	}

	void Effect::SetEffect( const Effect& effect ) {
		glUseProgram( effect.m_ShaderID );
		for( int i = 0; i < E_NUM_VARS; ++i ) {
			if( effect.m_ShaderVars[i] != GL_INVALID_VALUE ) {
				m_StateFunctions[i]( effect.m_ShaderVars[i] );
			}	
		}
	}
}
