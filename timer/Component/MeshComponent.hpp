#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <glm\glm.hpp>
#include <memory>

#include "Component.hpp"
#include "../Buffer/ArrayBuffer.hpp"


namespace Engine{
	namespace Meshes{
		struct Mesh;
	}
}


namespace Engine{
	namespace Textures{
		struct Texture;
	}
}

namespace Engine{
	namespace Entities{
		namespace Components{

			using Vec3Array = std::vector<glm::vec3>;
			using Vec2Array = std::vector<glm::vec2>;
			using UintArray = std::vector<unsigned int>;

			//TODO MAYBE REMOVE
			static enum MeshComponentType{
				MESH_INVALID = 0x00,
				MESH_COMMON = 0x01,
				MESH_BUILDING = 0x02,
				MESH_VEGETATION = 0x04,
				MESH_EMISSIVE = 0x08
			};

					
			class MeshComponent : public Component{
			private:
				unsigned int m_vertexArray = 0;
				Buffers::ArrayBuffer m_vertexBuffer;
				Buffers::ArrayBuffer m_indexBuffer;
				Meshes::Mesh *m_mesh = nullptr;
				MeshComponentType m_type = MESH_COMMON;

			private:
				void initBuffers();//TODO move to Meshes::Mesh		

			public:
				MeshComponent(std::unique_ptr<Meshes::Mesh> &&mesh) = delete;

				MeshComponent(const std::unique_ptr<Meshes::Mesh> &mesh) = delete;

				MeshComponent(const MeshComponent &meshComponent);

				MeshComponent(Meshes::Mesh &mesh, MeshComponentType type = MESH_COMMON);

				MeshComponent(Vec3Array &&vertices,
					Vec3Array &&normals = {}, Vec3Array &&tangents = {},
					Vec3Array &&colors = {}, Vec2Array &&uvs = {},
					std::vector<Textures::Texture> &&textures = {});

				~MeshComponent();


				const MeshComponentType &getType() const;

				void setType(MeshComponentType type);

				
				void setMesh(Meshes::Mesh &mesh);

				Meshes::Mesh &getMesh();


				void bindVertexArray() const;

				const unsigned int &getVertexBufferId() const;

				const unsigned int &getIndexBufferId() const;

			};
		}
	}
}

#endif //MESH_COMPONENT_HPP