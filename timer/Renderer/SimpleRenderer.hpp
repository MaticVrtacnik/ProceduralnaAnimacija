#ifndef SIMPLE_RENDERER_HPP
#define SIMPLE_RENDERER_HPP

#include <glm/glm.hpp>
#include <GL/glew.H>
#include "../Mesh/Mesh.hpp"

namespace Engine{
	namespace Meshes{

		void drawVector(
			const glm::vec3 &location, 
			const glm::vec3 &vector
		);

		void drawMesh(const Mesh *mesh);

	}
}


#endif //SIMPLE_RENDERER_HPP