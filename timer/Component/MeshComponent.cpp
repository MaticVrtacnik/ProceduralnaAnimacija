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


#include "MeshComponent.hpp"

#include "../Mesh/Mesh.hpp"
#include "../Define/PrintDefines.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			//For filling vertex data array buffers
			struct PaddedVertexData{
			public:
				glm::vec4 m_vertexColorR = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec4 m_normalColorG = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
				glm::vec4 m_tangentColorB = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
				glm::vec4 m_uv = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			};



			MeshComponent::MeshComponent(const MeshComponent &meshComponent) :
				m_mesh(meshComponent.m_mesh), m_type(meshComponent.m_type)
			{
				initBuffers();
			}

			MeshComponent::MeshComponent(Meshes::Mesh &mesh, MeshComponentType type) :
				m_mesh(&mesh), m_type(type)
			{
				initBuffers();
			}

			MeshComponent::MeshComponent(Vec3Array &&vertices, Vec3Array &&normals, 
				Vec3Array &&tangents, Vec3Array &&colors, Vec2Array &&uvs,
				std::vector<Textures::Texture> &&textures)
			{
				std::size_t _size = vertices.size();
				std::vector<Meshes::VertexData> _vertexData(_size);
				std::vector<unsigned int> _indices(_size);

				//TODO maybe
				/*for (std::size_t i = 0; i < _size; ++i){
					_indices[i] = i;
					_vertexData[i].m_vertex = std::move(vertices[i]);
					if (normals.size() > i) _vertexData[i].m_normal = std::move(normals[i]);
					if (tangents.size() > i) _vertexData[i].m_tangent = std::move(tangents[i]);
					if (colors.size() > i) _vertexData[i].m_color = std::move(colors[i]);
					if (uvs.size() > i) _vertexData[i].m_uv = std::move(uvs[i]);
				}*/

				assert(0);
				//m_mesh = std::make_unique<Mesh>(std::move(_vertexData), std::move(_indices), std::move(textures));
				//TODO REMOVE
				initBuffers();
			}

			MeshComponent::~MeshComponent(){
				glDeleteVertexArrays(1, &m_vertexArray);
				//PRINT("MESH COMPONENT DELETED")
				//buffers should delete themselves && mesh
			}


			void MeshComponent::initBuffers(){//TODO move to Meshes::Mesh
				glGenVertexArrays(1, &m_vertexArray);
				glBindVertexArray(m_vertexArray);

				std::vector<PaddedVertexData> _paddedVertexDataArray;
				_paddedVertexDataArray.reserve(m_mesh->m_vertexData.size());

				for (auto &_vertexData : m_mesh->m_vertexData){
					PaddedVertexData _paddedVertexData;

					glm::vec3 _color(_vertexData.m_color);
					_paddedVertexData.m_vertexColorR = glm::vec4(_vertexData.m_vertex, _color.r);
					_paddedVertexData.m_normalColorG = glm::vec4(_vertexData.m_normal, _color.r);
					_paddedVertexData.m_tangentColorB = glm::vec4(_vertexData.m_tangent, _color.r);
					_paddedVertexData.m_uv = glm::vec4(_vertexData.m_uv, 0.0f, 1.0f);

					_paddedVertexDataArray.emplace_back(std::move(_paddedVertexData));
				}


				m_vertexBuffer.loadBuffer(_paddedVertexDataArray, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
				const GLsizei _stride = sizeof(PaddedVertexData);
				std::size_t _offset = 0;

				//VERTEX + COLOR.R
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
					_stride, reinterpret_cast<const GLvoid*>(_offset));
				_offset += sizeof(glm::vec4);

				//NORMAL + COLOR.G
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
					_stride, reinterpret_cast<const GLvoid*>(_offset));
				_offset += sizeof(glm::vec4);

				//TANGENT + COLOR.B
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
					_stride, reinterpret_cast<const GLvoid*>(_offset));
				_offset += sizeof(glm::vec4);

				//UV
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
					_stride, reinterpret_cast<const GLvoid*>(_offset));


				m_indexBuffer.loadBuffer(m_mesh->m_indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

				glBindVertexArray(0);
			}


			const MeshComponentType &MeshComponent::getType() const{
				return m_type;
			}

			void MeshComponent::setType(MeshComponentType type){
				m_type = type;
			}


			void MeshComponent::setMesh(Meshes::Mesh &mesh){
				//TODO fix add destructor function .~
				m_vertexBuffer.~ArrayBuffer();
				m_indexBuffer.~ArrayBuffer();
				m_mesh = &mesh;
				initBuffers();
			}

			Meshes::Mesh &MeshComponent::getMesh(){
				return *m_mesh;
			}


			void MeshComponent::bindVertexArray() const{
				glBindVertexArray(m_vertexArray);
			}

			const unsigned int &MeshComponent::getVertexBufferId() const{
				return m_vertexBuffer.m_bufferId;
			}

			const unsigned int &MeshComponent::getIndexBufferId() const{
				return m_indexBuffer.m_bufferId;
			}

		}
	}
}
