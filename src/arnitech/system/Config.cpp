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
#include "renderer/Globals.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

atConfig::atConfig() : 
    m_FullScreen( false ),
    m_DrawDebug(false) {
    }

atConfig::~atConfig() {
}

bool Load( atConfig& config, const std::string& filename ) {
    std::ifstream inFile;
    inFile.open(filename.c_str());
    if (inFile) {
        std::string option;
        std::string value;
        while (inFile >> option) {
            if (option == "ViewportWidth") {
                inFile >> value;
                std::stringstream ss(value);
                ss >> config.m_RendererConfig.m_ViewportWidth;
                goto end;
            }

            if (option == "ViewportHeight") {
                inFile >> value;
                std::stringstream ss(value);
                ss >> config.m_RendererConfig.m_ViewportHeight;
                goto end;
            }

            if (option == "FullScreen") {
                inFile >> value;
                std::stringstream ss(value);
                ss >> config.m_FullScreen;
                goto end;
            }

            if (option == "DrawDebug") {
                inFile >> value;
                std::stringstream ss(value);
                ss >> config.m_DrawDebug;
                goto end;
            }

            if (option == "PixelAligned") {
                inFile >> value;
                std::stringstream ss(value);
                bool mode;
                ss >> mode;
                config.m_RendererConfig.m_RenderingMode = mode ? E_PIXEL_ALIGNED : E_GRID_ALIGNED;
                goto end;
            }

            if (option == "Directory") {
                inFile >> value;
                pathFinder.AddDirectory(value.c_str());
                goto end;
            }
            continue;
end:
            atContext::log->Print("Parsed option %s with value %s", option.c_str(), value.c_str());
        }
        inFile.close();
    }
    return true;
}

#undef PRINT_OPTION
