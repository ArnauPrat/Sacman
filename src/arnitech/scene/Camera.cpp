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

#include "Camera.hpp"
#include "system/Context.hpp"
#include "renderer/Renderer.hpp"
#include <cstring>

const char* atCamera::m_Type = "camera";

atCamera::atCamera( const char* name ) : atEntity( name )
{
    m_Extent = {0.0f, 0.0f};
}

atCamera::~atCamera() {
}

ATENTITY_UPDATE_DEF(atCamera) {
    atEntity* character = m_Level->GetEntity(m_Target);
    if( character ) {
        atConfig config = atContext::GetConfig();
        atVector2f frustrumExtent;
        if( config.m_RendererConfig.m_RenderingMode == E_PIXEL_ALIGNED ) {
            frustrumExtent.m_X = config.m_RendererConfig.m_ViewportWidth / (2.0f*static_cast<float>(config.m_RendererConfig.m_CellWidth));
            frustrumExtent.m_Y = config.m_RendererConfig.m_ViewportHeight / (2.0f*static_cast<float>(config.m_RendererConfig.m_CellHeight));
        } else {
            frustrumExtent.m_X = config.m_RendererConfig.m_GridWidth / 2.0f;
            frustrumExtent.m_Y = config.m_RendererConfig.m_GridHeight / 2.0f;
        }
        /** We compute the size of the level in world units to be renderer **/
        atVector2f levelExtent = { (m_LimitMax.m_X - m_LimitMin.m_X) / 2.0f , (m_LimitMax.m_Y - m_LimitMin.m_Y) / 2.0f };
        levelExtent.m_X = levelExtent.m_X < frustrumExtent.m_X ? frustrumExtent.m_X : levelExtent.m_X;
        levelExtent.m_Y = levelExtent.m_Y < frustrumExtent.m_Y ? frustrumExtent.m_Y : levelExtent.m_Y;
        atVector2f levelCenter = {m_LimitMin.m_X + levelExtent.m_X, m_LimitMin.m_Y + levelExtent.m_Y };

        /** Compute the lowerLeft and upperRight of que zone that delimits the possible camera position **/
        atVector2f lowerLeft = {levelCenter.m_X - levelExtent.m_X + frustrumExtent.m_X, levelCenter.m_Y - levelExtent.m_Y + frustrumExtent.m_Y};
        atVector2f upperRight = {levelCenter.m_X + levelExtent.m_X - frustrumExtent.m_X, levelCenter.m_Y + levelExtent.m_Y - frustrumExtent.m_Y};
        m_Position = character->Position();
        m_Position.m_X = m_Position.m_X < lowerLeft.m_X ? lowerLeft.m_X : m_Position.m_X;
        m_Position.m_X = m_Position.m_X > upperRight.m_X ? upperRight.m_X : m_Position.m_X;
        m_Position.m_Y = m_Position.m_Y < lowerLeft.m_Y ? lowerLeft.m_Y : m_Position.m_Y;
        atContext::m_Renderer.SetCameraPosition( m_Position );
    }
}

ATENTITY_ENTER_DEF(atCamera) {
    m_Level = level;
}

ATENTITY_LEAVE_DEF(atCamera) {
    m_Level = NULL;
}

ATENTITY_POSITION_DEF(atCamera)  {
    return m_Position;
}

ATENTITY_SET_POSITION_DEF(atCamera)  {
    m_Position = position;
}

ATENTITY_EXTENT_DEF(atCamera)  {
    return m_Extent;
}

ATENTITY_TYPE_DEF(atCamera)  {
    return m_Type;
}

void atCamera::SetTarget( const char* name ) {
    assert(std::strlen(name) < ATENTITY_NAME_LENGTH );
    std::strcpy(m_Target,name);
}

void atCamera::SetLimitMin( const atVector2f& min ) {
    m_LimitMin = min;
}

void atCamera::SetLimitMax( const atVector2f& max ) {
    m_LimitMax = max;
}


