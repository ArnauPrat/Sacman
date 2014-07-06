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

namespace dali {

    RendererConfig::RendererConfig() :
        m_RenderingMode( PIXEL_ALIGNED ),
        m_FullScreen( false ),
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
}
