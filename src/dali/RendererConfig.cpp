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

#include "RendererConfig.hpp"
#include <iostream>

namespace dali {

    RendererConfig::RendererConfig() :
        m_RenderingMode( GRID_ALIGNED ),
        m_CellWidth( 32 ),
        m_CellHeight( 32 ),
        m_GridWidth( 40 ),
        m_GridHeight( 22 ),
        m_ViewportWidth( 1280 ),
        m_ViewportHeight( 720 ) 
    {
    }

    RendererConfig::~RendererConfig(){
    }

    void PrintConfig( const RendererConfig& config ) {
        std::cout << std::endl;
        std::cout << "+++++++++++++++" << std::endl;
        std::cout << "Renderer Config" << std::endl;
        std::cout << "+++++++++++++++" << std::endl;
        std::cout << "Rendering Mode " << (config.m_RenderingMode==GRID_ALIGNED ? "GRID ALIGNED" : "PIXEL ALIGNED") << std::endl;
        std::cout << "Cell Width " << config.m_CellWidth << std::endl;
        std::cout << "Cell Height " << config.m_CellHeight << std::endl;
        std::cout << "GridWidth " << config.m_GridWidth << std::endl;
        std::cout << "GridHeight " << config.m_GridHeight << std::endl;
        std::cout << "ViewportWidth " << config.m_ViewportWidth << std::endl;
        std::cout << "ViewportHeight " << config.m_ViewportHeight << std::endl;
        std::cout << std::endl;
    }
}
