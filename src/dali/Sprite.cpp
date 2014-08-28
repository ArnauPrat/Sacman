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

#include "Globals.hpp"
#include "Sprite.hpp"
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <streambuf>
#include <string>
#include <cstring>

namespace dali {

    bool   Sprite::m_Initialized = false;
    Vector2fBuffer    Sprite::m_Vertices;
    IndexBuffer       Sprite::m_Indices;
     

    Sprite::Sprite() {
    }

    Sprite::~Sprite() {
    }

    void Sprite::Load( const char* fileName ) {

        if( !m_Initialized ) {
            math::Vector2f data[4];
            data[0].m_X = 0.0f;
            data[0].m_Y = 0.0f;
            data[1].m_X = 1.0f;
            data[1].m_Y = 0.0f;
            data[2].m_X = 1.0f;
            data[2].m_Y = 1.0f;
            data[3].m_X = 0.0f;
            data[3].m_Y = 1.0f;
            m_Vertices.AddData( data, 4 );
            unsigned short indexData[6];
            indexData[0] = 0;
            indexData[1] = 1;
            indexData[2] = 3;
            indexData[3] = 1;
            indexData[4] = 2;
            indexData[5] = 3;
            m_Indices.AddData( indexData, 6 );
            m_Initialized = true;
        }
        
        std::ifstream t(fileName);
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        Json::Value root;
        Json::Reader reader;
        bool parsedSuccess = reader.parse(str, root, false);
        if(!parsedSuccess) {
            std::cout << "Failed to parse JSON"<< std::endl 
                << reader.getFormattedErrorMessages()
                << std::endl;
            assert(false);
        }

        const Json::Value textureName = root["Texture"];
        m_Texture = dali::textureLoader.Load(textureName.asCString());
        const Json::Value texCoords = root["TexCoords"];
        assert(texCoords.size() <= 256 );
        math::Vector2f* texData = new math::Vector2f[texCoords.size()*4];
        int i = 0;
        for( Json::Value::iterator it = texCoords.begin(); it != texCoords.end(); ++it, ++i) {
            const Json::Value texCoords = (*it);
            math::Vector2f min = { (texCoords[0])[0].asFloat(), (texCoords[0])[1].asFloat() };
            math::Vector2f max = { (texCoords[1])[0].asFloat(), (texCoords[1])[1].asFloat() };
            texData[i*4] = {min.m_X, min.m_Y};
            texData[i*4 + 1] = {max.m_X, min.m_Y};
            texData[i*4 + 2] = {max.m_X, max.m_Y};
            texData[i*4 + 3] = {min.m_X, max.m_Y};
        }
        m_TexCoords.AddData( texData, texCoords.size()*4 );
        delete[] texData;

        const Json::Value animations = root["Animations"];
        for( unsigned i = 0; i < animations.size(); ++i ) {
            Animation auxAnim;
            const Json::Value animation = animations[i];
            const char* name = (animations[i])["Name"].asCString();
            assert(std::strlen(name) < ANIMATION_NAME_LENGTH);
            std::strcpy(auxAnim.m_Name, name);
            const Json::Value frames = animation["Frames"];
            assert( frames.size() <= ANIMATION_NUM_FRAMES );
            auxAnim.m_NumFrames = 0;
            for( unsigned j = 0; j < frames.size(); ++j ) {
                auxAnim.m_Frames[static_cast<int>(auxAnim.m_NumFrames++)] = static_cast<char>(frames[j].asInt());
            }
            m_Animations.push_back(auxAnim);
        }
    }

}
