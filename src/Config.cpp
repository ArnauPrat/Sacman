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
#include "dali/Globals.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace sacman {
    Config::Config() : 
        m_FullScreen( false ),
        m_DrawDebug(false) {
    }

    Config::~Config() {
    }

#define PRINT_OPTION(a,b) std::cout << "Parsed option " << a << " with " << b << " value. " <<  std::endl; 

    bool Load( Config& config, const std::string& filename ) {
        std::ifstream inFile;
        inFile.open(filename.c_str());
        std::string option;
        std::string value;
        while( inFile >> option ) {
            if( option == "ViewportWidth" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_RendererConfig.m_ViewportWidth;
                PRINT_OPTION(option, config.m_RendererConfig.m_ViewportWidth)
                continue;
            }

            if( option == "ViewportHeight" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_RendererConfig.m_ViewportHeight;
                PRINT_OPTION(option, config.m_RendererConfig.m_ViewportHeight);
                continue;
            }

            if( option == "FullScreen" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_FullScreen;
                PRINT_OPTION(option, config.m_FullScreen);
                continue;
            }

            if( option == "DrawDebug" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_DrawDebug;
                PRINT_OPTION(option, config.m_DrawDebug);
                continue;
            }

            if( option == "PixelAligned" ) {
                inFile >> value; 
                std::stringstream ss(value);
                bool mode;
                ss >> mode;
                config.m_RendererConfig.m_RenderingMode = mode ? dali::PIXEL_ALIGNED : dali::GRID_ALIGNED;
                PRINT_OPTION(option, config.m_DrawDebug);
                continue;
            }

            if( option == "Directory" ) {
                inFile >> value; 
                dali::pathFinder.AddDirectory( value.c_str() );
                PRINT_OPTION(option, value);
                continue;
            }

        }
        inFile.close();
        return true;
    }

#undef PRINT_OPTION
}
