/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "Skybox.hpp"

#include "../Cubemap/CubemapTexture.hpp"

//#define SKYBOX_ALL
//#define SKYBOX_BEST

namespace Engine{
	namespace Environment{

		Skybox::Skybox(){
			std::vector<std::pair<std::string, std::string>> _paths{
				{ "grey_sky", "bmp" }, //Debug sky
				{ "blue_sky", "png" }, //Debug sky		
				{ "white", "jpg" }, //Debug white color
				{ "black", "jpg" }, //Debug black color
			};

			/*m_cubemaps.push_back(Textures::createCubemap(std::vector<std::string>{
				"materials/mossy_ground/albedo.png",
				"materials/mossy_ground/albedo.png",
				"materials/mossy_ground/albedo.png",
				"materials/mossy_ground/albedo.png",
				"materials/mossy_ground/albedo.png",
				"materials/mossy_ground/albedo.png"
			}));*/

			/*m_cubemaps.push_back(Textures::createCubemap(std::vector<std::string>{
			"materials/plaster/albedo.png",
			"materials/plaster/albedo.png",
			"materials/plaster/albedo.png",
			"materials/plaster/albedo.png",
			"materials/plaster/albedo.png",
			"materials/plaster/albedo.png"
			}));*/

			m_cubemaps.reserve(_paths.size());

			for (auto &_pair : _paths){
				m_cubemaps.push_back(Textures::createCubemap(std::vector<std::string>{
					"skybox/" + _pair.first + "/right." + _pair.second,
					"skybox/" + _pair.first + "/left." + _pair.second,
					"skybox/" + _pair.first + "/top." + _pair.second,
					"skybox/" + _pair.first + "/bottom." + _pair.second,
					"skybox/" + _pair.first + "/front." + _pair.second,
					"skybox/" + _pair.first + "/back." + _pair.second
				}));
			}

			std::string _sponzaString = "../materials/sponza/floor/albedo.png";
			//m_cubemaps.push_back(Textures::createCubemap(std::vector<std::string>(6, _sponzaString)));


			/*m_cubemaps.push_back(Textures::createCubemap(std::vector<std::string>{
				"PBR_materials/brick3/albedo.png",
				"PBR_materials/brick3/albedo.png",
				"PBR_materials/brick3/albedo.png",
				"PBR_materials/brick3/albedo.png",
				"PBR_materials/brick3/albedo.png",
				"PBR_materials/brick3/albedo.png"
			}));*/


			const float scale = 100000.0;
			std::vector<glm::vec3> _vertices = {
				glm::vec3(+scale, +scale, +scale), glm::vec3(-scale, +scale, +scale), glm::vec3(-scale, -scale, +scale),
				glm::vec3(+scale, +scale, +scale), glm::vec3(-scale, -scale, +scale), glm::vec3(+scale, -scale, +scale),

				glm::vec3(-scale, +scale, -scale), glm::vec3(+scale, +scale, -scale), glm::vec3(+scale, -scale, -scale),
				glm::vec3(-scale, +scale, -scale), glm::vec3(+scale, -scale, -scale), glm::vec3(-scale, -scale, -scale),

				glm::vec3(-scale, +scale, +scale), glm::vec3(-scale, +scale, -scale), glm::vec3(-scale, -scale, -scale),
				glm::vec3(-scale, +scale, +scale), glm::vec3(-scale, -scale, -scale), glm::vec3(-scale, -scale, +scale),

				glm::vec3(+scale, -scale, -scale), glm::vec3(+scale, +scale, -scale), glm::vec3(+scale, +scale, +scale),
				glm::vec3(+scale, -scale, -scale), glm::vec3(+scale, +scale, +scale), glm::vec3(+scale, -scale, +scale),

				glm::vec3(-scale, -scale, +scale), glm::vec3(-scale, -scale, -scale), glm::vec3(+scale, -scale, -scale),
				glm::vec3(-scale, -scale, +scale), glm::vec3(+scale, -scale, -scale), glm::vec3(+scale, -scale, +scale),

				glm::vec3(+scale, +scale, -scale), glm::vec3(-scale, +scale, -scale), glm::vec3(-scale, +scale, +scale),
				glm::vec3(+scale, +scale, -scale), glm::vec3(-scale, +scale, +scale), glm::vec3(+scale, +scale, +scale)
			};


			glGenVertexArrays(1, &m_vertexArray);
			glBindVertexArray(m_vertexArray);

			m_vertexBuffer.loadBuffer(_vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindVertexArray(0);
		}

		Skybox::~Skybox(){
			for (auto &_cubemap : m_cubemaps){
				glDeleteTextures(1, &_cubemap);
			}
		}


		/*void Skybox::setCubemap(unsigned int cubemap){
			m_selectedCubemap = cubemap;
			//m_selectedIndex = 0;
		}*/

		const std::vector<unsigned int> &Skybox::getCubemaps() const{
			return m_cubemaps;
		}

		const unsigned int &Skybox::getCubemap(std::size_t index) const{
			ASSERT(index < m_cubemaps.size(), "Cubemap index out of range");

			return m_cubemaps[index]; //TODO
		}

		void Skybox::setNextCubemap(){
			if (++m_selectedIndex >= m_cubemaps.size()){
				m_selectedIndex = 0;
			}
		}

		void Skybox::setPreviousCubemap(){
			if (--m_selectedIndex < 0){
				m_selectedIndex = m_cubemaps.size() - 1;
			}
		}

		const unsigned int &Skybox::getSelectedCubemap() const{
			return getCubemap(m_selectedIndex);
		}

	}
}
