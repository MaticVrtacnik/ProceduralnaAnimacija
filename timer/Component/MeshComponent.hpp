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