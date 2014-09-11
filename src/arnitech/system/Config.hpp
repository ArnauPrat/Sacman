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

#ifndef ATCONFIG_H
#define ATCONFIG_H

#include "renderer/RendererConfig.hpp"
#include <string>

struct atConfig {
    atConfig();
    ~atConfig();

    atRendererConfig m_RendererConfig;      /**< @brief Configuration of the renderer.*/
    bool m_FullScreen;                          /**< @brief True to create the window fullscreen.*/
    bool m_DrawDebug;                           /**< @brief True to draw boxes. False otherwise.*/
};

/** @brief Loads a configuration file.
 *  @param filename The name of the file.
 *  @return True if the load was successfull. False otherwise.*/
bool Load( atConfig& config, const std::string& filename );

#endif
