#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "../Texture/TextureTypes.hpp"
//#include "../Define/Assert.hpp"
//#include "../Texture/Texture.hpp"


namespace Engine{ namespace Shaders{
	class ShaderBase;
	class Shader;	
} }

namespace Engine{ namespace Textures{
	struct Material;
	struct Texture;
} }

namespace Engine{ namespace Meshes{
	struct Mesh;
} }

namespace Engine{ namespace Animation{
	struct AnimationSkeleton;
} }


namespace Engine{
	class ResourceManager{
	private:
		std::unordered_map<std::string, 
			std::shared_ptr<Textures::Texture>> m_textureCache;

		std::unordered_map<std::string,
			std::shared_ptr<Animation::AnimationSkeleton>> m_skeletonCache;

		std::unordered_map<std::string, std::unordered_map<std::string, 
			std::vector<std::shared_ptr<Meshes::Mesh>>>> m_meshCache;

		std::unordered_map<std::string, 
			std::shared_ptr<Textures::Material>> m_materialCache;

		std::unordered_map<std::string, 
			std::shared_ptr<Shaders::ShaderBase>> m_shaderCache;

	public:
		ResourceManager();

		~ResourceManager();


		bool hasScene(const std::string &sceneKey);

		bool hasMesh(const std::string &sceneKey, const std::string &meshKey);

		bool hasMaterial(const std::string &key);

		bool hasSkeleton(const std::string &key);

		bool hasTexture(const std::string &key);

		bool hasShader(const std::string &key);

		/*template <typename... Args> void addMeshes(Args&&... args){
			std::vector<std::string> _paths { std::forward<Args>(args)... };

			for (auto &_path : _paths){
				addMesh(std::move(_path));
			}
		}*/


		//TODO get hierarchy tree from scene loader 
		//TODO store hierarchy into hashmap of trees

		//!!!!!!!!!!!!!!
		//TODO maybe convert to only mesh hashmap, save space compared to scene hashmap of hashmaps

	public: //MESH
		std::shared_ptr<Meshes::Mesh> getMeshPtr(const std::string &sceneKey, 
			const std::string &meshKey, std::size_t index = 0);	

		Meshes::Mesh &getMesh(const std::string &sceneKey, 
			const std::string &meshKey, std::size_t index = 0);

		std::shared_ptr<Meshes::Mesh> &addMesh(const std::string &sceneKey, 

			const std::string &meshKey, std::shared_ptr<Meshes::Mesh> mesh);

		std::unordered_map<std::string, std::vector<std::shared_ptr<Meshes::Mesh>>> 
			addSceneMeshes(const std::string &path);

		//std::vector<std::pair<std::string, std::shared_ptr<Meshes::Mesh>>> &addSceneMeshes(const std::string &path);
		
		//std::vector<std::pair<std::string, std::shared_ptr<Meshes::Mesh>>> &getSceneNamedMeshes(const std::string &sceneKey);
		
		std::vector<std::shared_ptr<Meshes::Mesh>> getSceneMeshes(const std::string &sceneKey);
		
		std::unordered_map<std::string, std::vector<std::shared_ptr<Meshes::Mesh>>> 
			getSceneMeshesMap(const std::string &sceneKey);
		
		void reloadMeshes();


	public: //SKELETON
		std::shared_ptr<Animation::AnimationSkeleton> getSkeletonPtr(const std::string &key);
		
		Animation::AnimationSkeleton &getSkeleton(const std::string &key);


	public: //TEXTURE
		std::shared_ptr<Textures::Texture> &getTexturePtr(const std::string &path);

		Textures::Texture &addTexture(const std::string &path, 
			Textures::TextureType textureType = Textures::TEXTURE_UNKNOWN);
		
		Textures::Texture &getTexture(const std::string &key);
		
		unsigned int getTextureId(const std::string &key);


	public: //MATERIAL
		std::shared_ptr<Textures::Material> getMaterialPtr(const std::string &key);
		
		Textures::Material &getMaterial(const std::string &key);
		
		Textures::Material &addMaterial(const std::string &key);
		
		Textures::Material &addMaterial(const std::string &key, Textures::Material &&material);

		void reloadMaterialConfigs();


	public: //SHADER
		template<typename T = Shaders::Shader>
		std::shared_ptr<T> getShaderPtr(const std::string &key)
		{
			if (hasShader(key)){
				return std::dynamic_pointer_cast<T>(m_shaderCache[key]);
			}

			addShader<T>(key);
			return std::dynamic_pointer_cast<T>(m_shaderCache[key]);
		}

		template<typename T = Shaders::Shader>
		T &getShader(const std::string &key)
		{
			auto &_shader = getShaderPtr<T>(key);
			//ASSERT(_shader != nullptr, "Invalid shader");

			return *_shader;
		}

		template<typename T>
		T &addShader(const std::string &name, const std::vector<std::string> &defines = {})
		{
			m_shaderCache[name] = std::make_unique<T>();

			auto &_shader = *static_cast<T*>(m_shaderCache[name].get());
			_shader.init(name, defines);

			return _shader;
		}
		
		void reloadShaders();

	};

}

#endif //RESOURCE_MANAGER_HPP