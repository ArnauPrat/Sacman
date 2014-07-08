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

#include "Config.hpp"
#include "Context.hpp"
#include <iostream>

#define CHECK_ARGUMENT_STRING(index, option,variable,setVariable) \
    if( strcmp(argv[index],option) == 0 ){ \
        setVariable = true; \
        if( (index+1) < argc ) { \
            variable = argv[index+1]; \
        } else { \
            printf( "Invalid options.\n" ); \
            return 1;\
        }\
    }

#define CHECK_ARGUMENT_FLOAT(index, option,variable,setVariable) \
    if( strcmp(argv[index],option) == 0 ){ \
        setVariable = true; \
        if( (index+1) < argc ) { \
            variable = atof(argv[index+1]); \
        } else { \
            printf( "Invalid options.\n" ); \
            return 1;\
        }\
    }

#define CHECK_ARGUMENT_INT(index, option,variable,setVariable) \
    if( strcmp(argv[index],option) == 0 ){ \
        setVariable = true; \
        if( (index+1) < argc ) { \
            variable = atoi(argv[index+1]); \
        } else { \
            printf( "Invalid options.\n" ); \
            return 1;\
        }\
    }

#define CHECK_FLAG(index, option,setVariable) \
    if( strcmp(argv[index],option) == 0 ){ \
        setVariable = true; \
    }

int main( int argc, char** argv ) {

    const char* configFilename;
    bool configFilenameSet = false; 
    for (int i = 1; i < argc; i++) {
        CHECK_ARGUMENT_STRING(i, "-c", configFilename, configFilenameSet)
    }

    sacman::Config config; 
    if( configFilenameSet ) {
        sacman::Load( config, configFilename );
    }

    sacman::Context::StartUp();

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        /* Check for new events */
        while(SDL_PollEvent(&event)) {
            /* If a quit event has been sent */
            if (event.type == SDL_QUIT) {
                /* Quit the application */
                quit = true;
            }
        }
    }
    sacman::Context::ShutDown();
    return 0;
}
