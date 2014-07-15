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

#ifndef GLSL_H
#define GLSL_H

#include<vector>
#include<string>

	namespace dali {
#define GLSLVARMACRO(varnumber, varstring) varnumber,
#define GLSLVARENUM \
                GLSLVARMACRO(E_PROJECTION_MATRIX,"ProjectionMatrix") \
                GLSLVARMACRO(E_VIEW_MATRIX,"ViewMatrix") \
                GLSLVARMACRO(E_MODEL_MATRIX,"ModelMatrix") \
                GLSLVARMACRO(E_TEX_DIFFUSE,"TexDiffuse") \
                GLSLVARMACRO(E_NUM_VARS,"Error") 

                enum GlslVar 
                {
                        GLSLVARENUM
                };
#undef GLSLVARMACRO

		extern const char * glslVarNames[];
		std::vector<GlslVar> ParseVars( int lines, char** src, int* sizes );
	}
#endif
