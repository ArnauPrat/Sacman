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

#ifndef ATRENDERERCONFIG_H
#define ATRENDERERCONFIG_H

enum atRenderingMode {
    E_PIXEL_ALIGNED,
    E_GRID_ALIGNED
};

struct atRendererConfig {
    atRenderingMode     m_RenderingMode;    /**< @brief The rendering mode: pixel or grid aligned.*/
    int                 m_CellWidth;        /**< @brief The width of a cell.*/
    int                 m_CellHeight;       /**< @brief The height of a cell.*/
    int                 m_GridWidth;        /**< @brief The width of the grid.*/
    int                 m_GridHeight;       /**< @brief The height of the grid.*/
    int                 m_ViewportWidth;    /**< @brief The width of the viewport.*/
    int                 m_ViewportHeight;   /**< @brief THe heighr of the viewport.*/
};

void Initialize( atRendererConfig& config );
void PrintConfig( const atRendererConfig& config );
#endif
