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


#include "VertexData.hpp"

#include <memory>


namespace Engine{
	namespace Meshes{

		VertexData::VertexData(){

		}

		VertexData::VertexData(const glm::vec3 &vertex, const glm::vec3 &normal,
			const glm::vec3 &tangent, const glm::vec3 &color, const glm::vec2 &uv
		) :
			m_vertex(vertex), 
			m_normal(normal),
			m_tangent(tangent), 
			m_color(color),
			m_uv(uv)
		{
		}

		VertexData::VertexData(glm::vec3 &&vertex, glm::vec3 &&normal,
			glm::vec3 &&tangent, glm::vec3 &&color, glm::vec2 &&uv
		) :
			m_vertex(std::move(vertex)), 
			m_normal(std::move(normal)),
			m_tangent(std::move(tangent)),
			m_color(std::move(color)),
			m_uv(std::move(uv))
		{
		}


		const glm::vec3 &VertexData::getVertex() const{
			return m_vertex;
		}

		void VertexData::setVertex(const glm::vec3 &vertex){
			m_vertex = vertex;
		}


		const glm::vec3 &VertexData::getNormal() const{
			return m_normal;
		}

		void VertexData::setNormal(const glm::vec3 &normal){
			m_normal = normal;
		}


		const glm::vec3 &VertexData::getTangent() const{
			return m_tangent;
		}

		void VertexData::setTangent(const glm::vec3 &tangent){
			m_tangent = tangent;
		}

		const glm::vec3 &VertexData::getColor() const{
			return m_color;
		}

		void VertexData::setColor(const glm::vec3 &color){
			m_color = color;
		}


		const glm::vec2 &VertexData::getUv() const{
			return m_uv;
		}

		void VertexData::setUv(const glm::vec2 &uv){
			m_uv = uv;
		}

	}
}
