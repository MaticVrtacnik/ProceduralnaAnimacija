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


#include "Mesh.hpp"

#include <string>
#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Meshes{

		Mesh::Mesh(){

		}

		Mesh::Mesh(const Mesh &mesh) :
			m_path(mesh.m_path),
			m_material(mesh.m_material),
			m_vertexData(mesh.m_vertexData),
			m_indices(mesh.m_indices),
			m_min(mesh.m_min),
			m_max(mesh.m_max),
			m_correctedLocation(mesh.m_correctedLocation)
		{
		}

		Mesh::Mesh(Mesh &&mesh) :
			m_path(std::move(mesh.m_path)),
			m_material(std::move(mesh.m_material)),
			m_vertexData(std::move(mesh.m_vertexData)),
			m_indices(std::move(mesh.m_indices)),
			m_min(std::move(mesh.m_min)),
			m_max(std::move(mesh.m_max)),
			m_correctedLocation(std::move(mesh.m_correctedLocation))
		{
		}

		Mesh::Mesh(
			std::string &&path,
			std::string &&material,
			std::vector<VertexData> &&v,
			std::vector<unsigned int> &&i,
			glm::vec3 &&min,
			glm::vec3 &&max,
			glm::vec3 &&correctedLocation
		) :
			m_path(std::move(path)),
			m_material(std::move(material)),
			m_vertexData(std::move(v)),
			m_indices(std::move(i)),
			m_min(std::move(min)),
			m_max(std::move(max)),
			m_correctedLocation(std::move(correctedLocation))
		{
		}

		Mesh::~Mesh(){
			PRINT("DELETED MESH");
		}

	}
}
