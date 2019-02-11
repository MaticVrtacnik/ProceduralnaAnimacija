#include "SimpleShapes.hpp"

#include "Mesh.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Meshes{

		std::unique_ptr<Mesh> getTriangle(unsigned int texture, float scale){
			std::vector<Textures::Texture> _textures = { Textures::Texture(texture) };
			std::vector<Meshes::VertexData> _vertexData;
			std::vector<unsigned int> _indices;

			Meshes::VertexData _tmpData;

			_tmpData.setVertex(glm::vec3(-scale, -scale, 0.0f));
			_tmpData.setUv(glm::vec2(0.0f, 0.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(0.0f, scale, 0.0f));
			_tmpData.setUv(glm::vec2(0.5f, 1.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(scale, -scale, 0.0f));
			_tmpData.setUv(glm::vec2(1.0f, 0.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_vertexData.push_back(_tmpData);

			_indices.insert(_indices.end(), {0, 1, 2});

			return std::make_unique<Mesh>(
				"triangle", "iron",
				std::move(_vertexData),
				std::move(_indices),
				glm::vec3(-scale, -scale, 0.0f), //min
				glm::vec3(scale, scale, 0.0f), //max
				glm::vec3(0.0f) //center
			);
		}

		std::unique_ptr<Mesh> getQuad(unsigned int texture, float scale){
			std::vector<Textures::Texture> _textures = { Textures::Texture(texture) };
			std::vector<Meshes::VertexData> _vertexData;
			std::vector<unsigned int> _indices;

			Meshes::VertexData _tmpData;

			_tmpData.setVertex(glm::vec3(-scale, -scale, 0.0));
			_tmpData.setUv(glm::vec2(1.0, 1.0));
			_tmpData.setNormal(glm::vec3(0.0, 0.0, 1.0));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scale, scale, 0.0));
			_tmpData.setUv(glm::vec2(1.0, 0.0));
			_tmpData.setNormal(glm::vec3(0.0, 0.0, 1.0));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(scale, scale, 0.0));
			_tmpData.setUv(glm::vec2(0.0, 0.0));
			_tmpData.setNormal(glm::vec3(0.0, 0.0, 1.0));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(scale, -scale, 0.0));
			_tmpData.setUv(glm::vec2(0.0, 1.0));
			_tmpData.setNormal(glm::vec3(0.0, 0.0, 1.0));
			_vertexData.push_back(_tmpData);

			_indices.insert(_indices.end(), {2, 1, 0, 0, 3, 2});

			return std::make_unique<Mesh>(
				"quad", "iron",
				std::move(_vertexData), 
				std::move(_indices),
				glm::vec3(-scale, -scale, 0.0f), //min
				glm::vec3(scale, scale, 0.0f), //max
				glm::vec3(0.0f) //center
			);
		}

		std::unique_ptr<Mesh> getCircle(unsigned int texture, float scale, float slices){
			std::vector<Textures::Texture> _textures = { Textures::Texture(texture) };
			std::vector<Meshes::VertexData> _vertexData;
			std::vector<unsigned int> _indices;

			Meshes::VertexData _tmpData;

			_tmpData.setVertex(glm::vec3(0.0f, 0.0f, 0.0f));
			_tmpData.setUv(glm::vec2(0.5f, 0.5f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_vertexData.push_back(_tmpData);

			float _sliceAngle = 360.0f / slices;
			for (int i = 0; i < slices; ++i){
				float _rad = glm::radians(i * _sliceAngle);
				float _x = cos(_rad);
				float _y = sin(_rad);

				_tmpData.setVertex(glm::vec3(_x, _y, 0.0f) * scale);
				_tmpData.setUv(glm::vec2(_x + 1.0f, _y + 1.0f) / 2.0f);
				_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
				_vertexData.push_back(_tmpData);
			}

			for (std::size_t i = 0; i < _vertexData.size(); ++i){
				_indices.push_back(0);
				_indices.push_back(i);
				_indices.push_back(i + 1); //TODO fix
			}

			return std::make_unique<Mesh>(
				"circle", "iron",
				std::move(_vertexData),
				std::move(_indices),
				glm::vec3(-scale, -scale, 0.0f), //min
				glm::vec3(scale, scale, 0.0f), //max
				glm::vec3(0.0f) //center
			);
		}

		std::unique_ptr<Mesh> getCube(unsigned int texture, float scale){
			return getCube(texture, scale, scale, scale);
		}

		std::unique_ptr<Mesh> getCube(unsigned int texture, float scaleX, float scaleY, float scaleZ){
			std::vector<Textures::Texture> _textures = { Textures::Texture(texture) };
			std::vector<Meshes::VertexData> _vertexData;
			Meshes::VertexData _tmpData;


			//FRONT
			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);


			//BACK
			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);


			//LEFT
			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);


			//RIGHT
			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(0.0f, 1.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);


			//BOTTOM
			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, -scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);


			//TOP
			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 0.0f));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, -scaleZ / 2.0f));
			_tmpData.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(1.0f, 0.0f)); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f)); 
			_tmpData.setUv(glm::vec2(0.0f, 1.0f));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(+scaleX / 2.0f, +scaleY / 2.0f, +scaleZ / 2.0f)); 
			_tmpData.setNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			_tmpData.setTangent(glm::vec3(1.0f, 0.0f, 0.0f));
			_tmpData.setUv(glm::vec2(1.0f, 1.0f)); 
			_vertexData.push_back(_tmpData);
		

			std::vector<unsigned int> _indices;
			_indices.reserve(_vertexData.size());
			for (unsigned int i = 0; i < _vertexData.size(); ++i){
				_indices.push_back(i);
			}


			return std::make_unique<Mesh>(
				"cube", "titanium",
				std::move(_vertexData),
				std::move(_indices),
				glm::vec3(-scaleX / 2.0f, -scaleY / 2.0f, -scaleZ / 2.0f), //min
				glm::vec3(scaleX / 2.0f, scaleY / 2.0f, scaleZ / 2.0f), //max
				glm::vec3(0.0f) //center
			);
		}

		std::unique_ptr<Mesh> getSphere(unsigned int texture, float scale, float slices, float stacks){
			const float R = 1.0f / (float)(stacks - 1);
			const float S = 1.0f / (float)(slices - 1);

			std::vector<Textures::Texture> _textures = { Textures::Texture(texture) };
			std::vector<Meshes::VertexData> _vertexData;
			std::vector<unsigned int> _indices;

			Meshes::VertexData _tmpData;

			/*for (float r = 0.0f; r < stacks; r++){
				for (float s = 0.0f; s < slices; s++){
					const float y = static_cast<float>(sin(-M_PI * 2 + M_PI * r * R));
					const float x = static_cast<float>(cos(2 * M_PI * s * S) * sin(M_PI * r * R));
					const float z = static_cast<float>(sin(2 * M_PI * s * S) * sin(M_PI * r * R));

					_tmpData.setUv(glm::vec2(s * S, r * R);
					_tmpData.setVertex(glm::vec3(x, y, z) * scale;
					_tmpData.setNormal(glm::vec3(x, y, z);
					_vertexData.push_back(_tmpData);
				}
			}

			for (float r = 0.0f; r < stacks - 1; r++){
				for (float s = 0.0f; s < slices - 1; s++) {
					_indices.push_back(static_cast<unsigned int>(r * slices + s));
					_indices.push_back(static_cast<unsigned int>(r * slices + (s + 1)));
					_indices.push_back(static_cast<unsigned int>((r + 1) * slices + (s + 1)));
					_indices.push_back(static_cast<unsigned int>((r + 1) * slices + s));
				}
			}*/



			float t = (1.0f + sqrt(5.0f)) / 2.0f;
			t *= scale / 2.0f;

			_tmpData.setVertex(glm::vec3(-scale / 2.0f, t, 0.0f)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex())); 
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(scale / 2.0f, t, 0.0f));
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-scale / 2.0f, -t, 0.0f));
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(scale / 2.0f, -t, 0.0f)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(0.0f, -scale / 2.0f, t)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(0.0f, scale / 2.0f, t));
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(0.0f, -scale / 2.0f, -t));
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(0.0f, scale / 2.0f, -t));
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(t, 0.0f, -scale / 2.0f)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(t, 0.0f, scale / 2.0f)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-t, 0.0f, -scale / 2.0f)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);

			_tmpData.setVertex(glm::vec3(-t, 0.0f, scale / 2.0f)); 
			_tmpData.setNormal(glm::normalize(_tmpData.getVertex()));
			_vertexData.push_back(_tmpData);


			_indices.insert(_indices.begin(), 
			{ 
				0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
				1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
				3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
				4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
			});


			return std::make_unique<Mesh>(
				"sphere", "iron",
				std::move(_vertexData),
				std::move(_indices),
				glm::vec3(-scale, -scale, -scale), //min
				glm::vec3(scale, scale, scale), //max
				glm::vec3(0.0f) //center
			);
		}

	}
}