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

#include "SpriteRenderer.hpp"
#include <cstring>
#include <iostream>

namespace dali {


    SpriteRenderer::SpriteRenderer( const Sprite& sprite ) : 
        m_Sprite( sprite ), 
        m_TotalTime( 0 ),
        m_ElapsedTime( 0 ),
        m_AnimationRunning( false ),
        m_AnimationIndex( 0 ),
        m_Loop( false )
    {
    }

    SpriteRenderer::~SpriteRenderer() {
    }

    void SpriteRenderer::Draw( dali::Renderer& renderer, 
            const double elapsedTime,
            const int depth,
            const math::Vector2f& position, 
            const math::Vector2f& scale) {

        void* frame = 0;
        if( m_AnimationRunning ) {
            m_ElapsedTime += elapsedTime;
            if( m_ElapsedTime >= m_TotalTime && !m_Loop ) { 
                m_ElapsedTime = 0;
                m_TotalTime = 1.0;  
                m_Loop = false;
                m_AnimationRunning = false;
            } else if( m_ElapsedTime >= m_TotalTime && m_Loop ) {
                m_ElapsedTime -= m_TotalTime;
            }
            const Sprite::Animation& animation = m_Sprite.m_Animations[m_AnimationIndex];
            frame = reinterpret_cast<void*>(animation.m_Frames[static_cast<int>((m_ElapsedTime / (double)m_TotalTime)*animation.m_NumFrames)]*sizeof(math::Vector2f)*4);
        }
        renderer.Draw( &m_Sprite.m_Vertices, &m_Sprite.m_TexCoords, &m_Sprite.m_Indices, m_Sprite.m_Texture, depth, frame, position, scale );
    }

    void SpriteRenderer::LaunchAnimation( const char* name, double totalTime, bool loop ) {
        m_AnimationIndex = GetAnimationIndex( name );
        m_TotalTime = totalTime;
        m_ElapsedTime = 0.0;
        m_Loop = loop;
        m_AnimationRunning = true;
    }

    void SpriteRenderer::StopAnimation() {
        m_ElapsedTime = 0;
        m_TotalTime = 1.0;  
        m_Loop = false;
        m_AnimationRunning = false;
    }

    bool SpriteRenderer::IsAnimationRunning( const char* name ) {
        return m_AnimationRunning && GetAnimationIndex( name ) == m_AnimationIndex;
    }

    char SpriteRenderer::GetAnimationIndex( const char* name ) {
        for( unsigned int i = 0; i < m_Sprite.m_Animations.size(); ++i ) {
            const Sprite::Animation& animation = m_Sprite.m_Animations[i];
            if( std::strcmp( name, animation.m_Name ) == 0 ) {
                return i;
            }
        }
        return 0;
    }
}
