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
#include <iostream>
#include <string.h>
#include <string>

namespace dali {
#define GLSLVARMACRO(varnumber, varstring) varstring,
		const char * glslVarNames[] =
		{
			GLSLVARENUM	
		};
#undef GLSLVARMACRO

        std::vector<GlslVar> ParseVars( int lines, char** src, int* sizes ) {
			std::vector<GlslVar> retVec;
			for( int i = 0; i < lines; i++ ) {
				char* currentLine = src[i];
				if( strstr( currentLine, "main") != NULL ) {
					break;
				}
				if( strstr(currentLine,"uniform") != NULL ) {
					size_t semicolonPos = strcspn(currentLine,";");
					int j;
					for( j = static_cast<int>(semicolonPos - 1); j >= 0; j-- ) {
						if( currentLine[j] == ' ' ) {
							break;
						}
					}
					j++;
					bool found = false;
					for( int k = 0; k < E_NUM_VARS; k++ )  {
						if( strncmp( glslVarNames[k], &currentLine[j], semicolonPos - 1 - j ) == 0 ) {
							retVec.push_back(static_cast<GlslVar>(k));
							found = true;
							break;
						}
					}
                    if( !found ) {
                        std::cout << "Unknown variable " << std::string(currentLine) << std::endl;
                    }
				}
			}
			return retVec;
		}
	}

