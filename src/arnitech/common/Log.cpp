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

#include "Log.hpp"
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>


#define VARIADIC_BUFFER_SIZE 1024
#define EXTRACT_VARIADIC( buffer, message ) \
                            va_list larg; \
                            va_start(larg, message); \
                            vsprintf(buffer,message, larg); \
                            va_end(larg);

#define TIMED_MESSAGE( message ) \
                             time_t rawtime;  \
                             time(&rawtime); \
                             char timestr[128];\
                             const char* timeptr = ctime(&rawtime); \
                             size_t length = strlen(timeptr); \
                             std::strncpy(timestr,timeptr,length); \
                             timestr[length-1] = '\0'; \
                             fprintf(fd,"%s %s\n",timestr, message);


atLog::atLog(const char* fileName) {
    fd = fopen(fileName, "a");
    TIMED_MESSAGE(":Started execution");
}

atLog::~atLog() {
    TIMED_MESSAGE(":Finished execution");
    fclose(fd);
}

void atLog::Print(const char* message, ...) {
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
        Message("LOG", buffer);
}
void atLog::Error(const char* message, ...) {
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
      Message("ERROR", buffer);
}

void atLog::Warning(const char* message, ...) {
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
        Message("WARNING", buffer);
}

void atLog::Message(const char* type, const char* message) {
    char buffer[1024];
    sprintf(buffer, ":%s:%s",type,message);
    assert(strlen(buffer) < 1024);
    TIMED_MESSAGE(buffer);
}

#undef EXTRACT_VARIADIC
#undef VARIADIC_BUFFER_SIZE
