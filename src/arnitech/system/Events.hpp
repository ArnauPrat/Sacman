/*Sacman is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Sacman is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#ifndef ATEVENTS_H
#define ATEVENTS_H
enum atEventType {
    E_KEYBOARD
};

enum atKeyCode {
    E_K_UP,
    E_K_DOWN,
    E_K_LEFT,
    E_K_RIGHT,
    E_K_SPACE,
    E_K_E,
    E_K_ESC
};

enum atKeyEventType {
    K_PRESSED,
    K_RELEASED
};

struct atKeyEvent {
    atKeyCode         m_KCode;
    atKeyEventType    m_KEType;
};
#endif
