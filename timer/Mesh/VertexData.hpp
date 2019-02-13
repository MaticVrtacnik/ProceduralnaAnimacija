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


#ifndef VERTEX_DATA_HPP
#define VERTEX_DATA_HPP

#include <glm\glm.hpp>


namespace Engine{
	namespace Meshes{

		struct VertexData{
		public:
			glm::vec3 m_vertex = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 m_tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			glm::vec3 m_color = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec2 m_uv = glm::vec2(0.0f, 0.0f);

		public:
			VertexData();

			VertexData(const glm::vec3 &vertex, const glm::vec3 &normal,
				const glm::vec3 &tangent, const glm::vec3 &color, const glm::vec2 &uv);

			VertexData(glm::vec3 &&vertex, glm::vec3 &&normal,
				glm::vec3 &&tangent, glm::vec3 &&color, glm::vec2 &&uv);

		public:
			const glm::vec3 &getVertex() const;

			void setVertex(const glm::vec3 &vertex);


			const glm::vec3 &getNormal() const;

			void setNormal(const glm::vec3 &normal);


			const glm::vec3 &getTangent() const;

			void setTangent(const glm::vec3 &tangent);


			const glm::vec3 &getColor() const;

			void setColor(const glm::vec3 &color);


			const glm::vec2 &getUv() const;

			void setUv(const glm::vec2 &uv);

		};

	}
}

#endif //VERTEX_DATA_HPP