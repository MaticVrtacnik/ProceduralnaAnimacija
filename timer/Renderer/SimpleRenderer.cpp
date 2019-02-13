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


#include "SimpleRenderer.hpp"

#include "../Define/Assert.hpp"

namespace Engine{
	namespace Meshes{

		void drawVector(
			const glm::vec3 &location,
			const glm::vec3 &vector)
		{
			glm::vec3 _start = location;
			glm::vec3 _end = _start + vector * 1000.0f;

			glBegin(GL_LINES);
			glVertex3f(_start.x, _start.y, _start.z);
			glVertex3f(_end.x, _end.y, _end.z);
			glEnd();
		}

		void drawMesh(const Mesh *mesh){
			ASSERT(mesh != nullptr, "Mesh is a null pointer");

			GLfloat _color[] = { 1, 1, 0, 0, 1, 0 };
			int _c = 0;

			glPushMatrix();
			{
				glScalef(100, 100, 100);
				glBegin(GL_TRIANGLES);

				for (auto &_index : mesh->m_indices){
					auto &_vertex = mesh->m_vertexData[_index].getVertex();

					glColor3f(_color[++_c % 6], _color[_c % 6], _color[_c % 6]);
					glVertex3f(_vertex.x, _vertex.y, _vertex.z);
				}

				glEnd();
			}
			glPopMatrix();
			
		}

	}
}
