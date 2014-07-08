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
#include <fstream>
#include <sstream>

namespace sacman {
    Config::Config() : 
        m_FullScreen( false ) {
    }

    Config::~Config() {
    }


    bool Load( Config& config, const std::string& filename ) {
        std::ifstream inFile;
        inFile.open(filename.c_str());
        std::string option;
        std::string value;
        while( inFile >> option ) {
            if( inFile == "ViewportWidth" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_RendererConfig.m_ViewportWidth;
                continue;
            }

            if( inFile == "ViewportHeight" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_RendererConfig.m_ViewportHeight;
                continue;
            }

            if( inFile == "FullScreen" ) {
                inFile >> value; 
                std::stringstream ss(value);
                ss >> config.m_FullScreen;
                continue;
            }
        }
        inFile.close();
        return true;
    }
}
