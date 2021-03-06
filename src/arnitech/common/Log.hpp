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

#ifndef ATLOG_H
#define ATLOG_H

#include "common/Platform.hpp"
#include "common/Macros.hpp"
#include <cstdio>

class atLog {
    ATNON_COPYABLE(atLog)
    public:
        ATEXPORT atLog(const char* logFileName);
        ATEXPORT ~atLog();

        ATEXPORT void Print( const char* message, ... );
        ATEXPORT void Error( const char* message, ... );
        ATEXPORT void Warning( const char* message, ... );

    private:
        void Message(const char* type, const char* message );
        FILE* fd;
};

#endif
