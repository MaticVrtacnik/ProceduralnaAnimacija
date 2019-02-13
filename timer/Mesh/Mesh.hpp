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


#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <memory>
#include "VertexData.hpp"


namespace Engine{
	namespace Meshes{

		struct Mesh{
		public:
			std::string m_path = "";
			std::string m_material = "";
			std::vector<VertexData> m_vertexData;
			std::vector<unsigned int> m_indices;
			glm::vec3 m_min = glm::vec3(-1.0f);
			glm::vec3 m_max = glm::vec3(1.0f);
			glm::vec3 m_correctedLocation = glm::vec3(0.0f);

		public:
			Mesh();

			Mesh(const Mesh &mesh);

			Mesh(Mesh &&mesh);

			Mesh(std::string &&path, std::string &&material, std::vector<VertexData> &&v,
				std::vector<unsigned int> &&i, glm::vec3 &&min, glm::vec3 &&max, 
				glm::vec3 &&correctedLocation);

			~Mesh();

		};

	}
}

#endif //MESH_HPP