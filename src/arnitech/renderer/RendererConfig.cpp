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

#include "system/Context.hpp"
#include "RendererConfig.hpp"
#include <iostream>

void Initialize( atRendererConfig& config ) {
    config.m_RenderingMode = E_GRID_ALIGNED;
    config.m_CellWidth = 32;
    config.m_CellHeight = 32;
    config.m_GridWidth = 40;
    config.m_GridHeight = 22;
    config.m_ViewportWidth = 1280;
    config.m_ViewportHeight = 720;
}

void PrintConfig( const atRendererConfig& config ) {
    atContext::log->Print("+++++++++++++++");
    atContext::log->Print("Renderer Config");
    atContext::log->Print("+++++++++++++++");
    atContext::log->Print("Rendering Mode %s",config.m_RenderingMode==E_GRID_ALIGNED ? "GRID ALIGNED" : "PIXEL ALIGNED");
    atContext::log->Print("Cell Width %d", config.m_CellWidth);
    atContext::log->Print("Cell Height %d", config.m_CellHeight); 
    atContext::log->Print("GridWidth %d", config.m_GridWidth); 
    atContext::log->Print("GridHeight %d", config.m_GridHeight); 
    atContext::log->Print("ViewportWidth %d",  config.m_ViewportWidth);
    atContext::log->Print("ViewportHeight %d", config.m_ViewportHeight);
}
