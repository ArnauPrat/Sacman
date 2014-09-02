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

#ifndef EVENTS_H
#define EVENTS_H
namespace sacman {
    enum EventType {
        KEYBOARD
    };

    enum KeyCode {
        K_UP,
        K_DOWN,
        K_LEFT,
        K_RIGHT,
        K_SPACE,
        K_E,
        K_ESC
    };

    enum KeyEventType {
        K_PRESSED,
        K_RELEASED
    };

    struct KeyEvent {
        KeyCode         m_KCode;
        KeyEventType    m_KEType;
    };
}
#endif
