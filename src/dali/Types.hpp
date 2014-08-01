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

#ifndef TYPES_H
#define TYPES_H

namespace dali {
	struct Vector2f {
		float m_X;
		float m_Y;
	};

	struct TexQuad {
		Vector2f m_Min;
		Vector2f m_Max;
		Vector2f m_TexMin;
		Vector2f m_TexMax;
	};
}

#endif
