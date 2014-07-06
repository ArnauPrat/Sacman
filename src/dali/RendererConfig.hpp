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

#ifndef RENDERERCONFIG_H
#define RENDERERCONFIG_H

namespace dali {

    enum RenderingMode {
        PIXEL_ALIGNED,
        GRID_ALIGNED
    };

    struct RendererConfig {
            RendererConfig();
            ~RendererConfig();

            RenderingMode   m_RenderingMode;
            bool            m_FullScreen; 
            int             m_CellWidth;
            int             m_CellHeight;
            int             m_GridWidth;
            int             m_GridHeight;
            int             m_ViewportWidth;
            int             m_ViewportHeight;

    };
}
#endif
