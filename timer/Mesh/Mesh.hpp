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