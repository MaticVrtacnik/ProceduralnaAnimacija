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