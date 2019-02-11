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
