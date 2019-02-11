#include "ResourceManager.hpp"

#include "../Define/Assert.hpp"
#include "../Texture/Texture.hpp"
#include "../Texture/Material.hpp"
#include "../Shader/Shader.hpp"
#include "../Mesh/SceneLoader.hpp"
#include "../Mesh/SimpleShapes.hpp"
#include "../Mesh/Mesh.hpp"


namespace Engine{

	ResourceManager::ResourceManager(){
		/*addMaterial("grass");
		addMaterial("rock");
		addMaterial("mossy_ground");
		//addMaterial("coral");
		addMaterial("forest_dirt");
		addMaterial("gold");*/
		//addMaterial("iron");
		//addMaterial("forest_trail");

		addMesh("primitives", "sphere", Meshes::getSphere(0, 1.0f, 10.0f, 10.0f));
		addMesh("primitives", "cube", Meshes::getCube(0, 1.0f));
		addMesh("primitives", "triangle", Meshes::getTriangle(0, 1.0f));
		addMesh("primitives", "circle", Meshes::getCircle(0, 1.0f, 10.0f));
		addMesh("primitives", "quad", Meshes::getQuad(0, 1.0f));
	}

	ResourceManager::~ResourceManager(){
		for (auto &_texture : m_textureCache){
			glDeleteTextures(1, &_texture.second->m_textureId);
		}
	}



	bool ResourceManager::hasScene(const std::string &sceneKey){
		return m_meshCache.find(sceneKey) != m_meshCache.end();
	}

	bool ResourceManager::hasMesh(const std::string &sceneKey, const std::string &meshKey){
		if (!hasScene(sceneKey))return false;

		auto &_sceneMeshes = m_meshCache[sceneKey];
		return _sceneMeshes.find(meshKey) != _sceneMeshes.end();
	}

	bool ResourceManager::hasMaterial(const std::string &name){
		return m_materialCache.find(name) != m_materialCache.end();
	}

	bool ResourceManager::hasSkeleton(const std::string &path){
		return m_skeletonCache.find(path) != m_skeletonCache.end();
	}

	bool ResourceManager::hasTexture(const std::string &path){
		return m_textureCache.find(path) != m_textureCache.end();
	}

	bool ResourceManager::hasShader(const std::string &path){
		return m_shaderCache.find(path) != m_shaderCache.end();
	}



	std::unordered_map<std::string, std::vector<std::shared_ptr<Meshes::Mesh>>>
		ResourceManager::addSceneMeshes(const std::string &path)
	{
			if (hasScene(path)) return getSceneMeshesMap(path);

			auto _meshData = Meshes::loadSceneData(path);
			if (!_meshData.isValid()) {
				ASSERT(0, "Mesh with path '" + path + "' is invalid");
				return std::unordered_map<std::string,
					std::vector<std::shared_ptr<Meshes::Mesh>>>();
			}

			if (_meshData.hasSkeleton()){
				m_skeletonCache[path] = std::move(_meshData.m_skeleton);
			}

			for (auto &_meshPropertiesData : _meshData.m_meshes){
				m_meshCache[path][_meshPropertiesData.m_name].push_back(_meshPropertiesData.m_mesh);

				const std::string _materialName(_meshPropertiesData.m_mesh->m_material);
				if (hasMaterial(_materialName))continue;

				auto &_newMaterial = addMaterial(_materialName);
				for (auto &_texture : _meshPropertiesData.m_textureData){
					auto _type = static_cast<Textures::TextureType>(_texture.m_type);

					/*if (!_texture.m_path.empty() && !hasTexture(_texture.m_path)){
						m_textureCache[_texture.m_path] = std::make_shared
							<Textures::Texture>(_texture.m_path, true, _type);
					}*/

					if (!_texture.m_path.empty() && !_newMaterial.hasTexture(_type)){
						m_textureCache[_texture.m_path] = std::make_shared
							<Textures::Texture>(_texture.m_path, true, _type);

						_newMaterial.setTexture(m_textureCache[_texture.m_path]);
					}			
				}
			}

			return getSceneMeshesMap(path);
		}

	std::shared_ptr<Meshes::Mesh> &ResourceManager::addMesh(const std::string &sceneKey,
		const std::string &meshKey, std::shared_ptr<Meshes::Mesh> mesh)
	{
		//ASSERT(!hasMesh(sceneKey, meshKey), "Mesh already exists");
		m_meshCache[sceneKey][meshKey].push_back(mesh);

		return m_meshCache[sceneKey][meshKey].back();
	}

	/*std::vector<std::pair<std::string, std::shared_ptr<Meshes::Mesh>>>
		&ResourceManager::getSceneNamedMeshes(const std::string &sceneKey)
		{
		if (!hasScene(sceneKey)) return addSceneMeshes(sceneKey);
		else{
		auto &_scene = m_meshCache[sceneKey];

		std::vector<std::pair<std::string, std::shared_ptr<Meshes::Mesh>>> _namedMeshes;
		_namedMeshes.reserve(_scene.size());

		for (auto &_mesh : _scene){
		_namedMeshes.push_back(std::make_pair(_mesh.first, _mesh.second));
		}

		return _namedMeshes;
		}
		}*/

	std::vector<std::shared_ptr<Meshes::Mesh>> ResourceManager::getSceneMeshes(
		const std::string &sceneKey)
	{
		if (!hasScene(sceneKey))addSceneMeshes(sceneKey);

		auto &_meshesMap = m_meshCache[sceneKey];

		std::vector<std::shared_ptr<Meshes::Mesh>> _sceneMeshes;
		_sceneMeshes.reserve(_meshesMap.size());

		for (auto &_meshes : _meshesMap){
			for (auto &_mesh : _meshes.second){
				_sceneMeshes.push_back(_mesh);
			}
		}

		return _sceneMeshes;
	}

	std::unordered_map<std::string, std::vector<std::shared_ptr<Meshes::Mesh>>>
		ResourceManager::getSceneMeshesMap(const std::string &sceneKey)
	{
			return m_meshCache[sceneKey];
			/*return hasScene(sceneKey) ? m_meshCache[sceneKey] :
				std::unordered_map<std::string, std::shared_ptr<Meshes::Mesh>>();*/
		}

	std::shared_ptr<Meshes::Mesh> ResourceManager::getMeshPtr(
		const std::string &sceneKey,
		const std::string &meshKey,
		std::size_t index)
	{
		if (!hasMesh(sceneKey, meshKey))return nullptr;

		auto &_meshes = m_meshCache[sceneKey][meshKey];
		return index < _meshes.size() ? _meshes[index] : nullptr;
	}

	Meshes::Mesh &ResourceManager::getMesh(
		const std::string &sceneKey,
		const std::string &meshKey,
		std::size_t index)
	{
		auto &_mesh = getMeshPtr(sceneKey, meshKey, index);
		ASSERT(_mesh != nullptr, "Mesh is a null pointer");

		return *_mesh;
	}

	void ResourceManager::reloadMeshes(){
		//TODO reload meshes[]
	}



	std::shared_ptr<Animation::AnimationSkeleton>
		ResourceManager::getSkeletonPtr(const std::string &key)
	{
			return hasSkeleton(key) ? m_skeletonCache[key] : nullptr;
		}

	Animation::AnimationSkeleton &ResourceManager::getSkeleton(const std::string &key){
		auto &_skeleton = getSkeletonPtr(key);
		ASSERT(_skeleton != nullptr, "Invalid skeleton");

		return *_skeleton;
	}

	std::shared_ptr<Textures::Texture> &ResourceManager::getTexturePtr(const std::string &path){
		if (!hasTexture(path)){
			addTexture(path);
		}

		return m_textureCache[path];

		//return hasTexture(path) ? m_textureCache[path] : nullptr;
	}



	Textures::Texture &ResourceManager::addTexture(
		const std::string &path, Textures::TextureType textureType)
	{
		if (hasTexture(path)){
			auto &_texture = m_textureCache[path];
			if (_texture != nullptr) return *_texture;
			else ASSERT(0, "Texture '" + path + "' is a null pointer");
		}

		m_textureCache[path] = std::make_shared<Textures::Texture>(path, true, textureType);
		return *m_textureCache[path];
	}

	Textures::Texture &ResourceManager::getTexture(const std::string &key){
		auto &_texturePtr = getTexturePtr(key);
		ASSERT(_texturePtr != nullptr, "Texture '" + key + "' is a null pointer");

		return *_texturePtr;
	}

	unsigned int ResourceManager::getTextureId(const std::string &key){
		return getTexture(key).m_textureId;
	}



	std::shared_ptr<Textures::Material> ResourceManager::getMaterialPtr(const std::string &key){
		if (hasMaterial(key)) return m_materialCache[key];

		addMaterial(key);
		return m_materialCache[key];
	}

	Textures::Material &ResourceManager::getMaterial(const std::string &key){
		auto &_material = getMaterialPtr(key);
		ASSERT(_material != nullptr, "Invalid material");

		return *_material;
	}

	Textures::Material &ResourceManager::addMaterial(const std::string &key){
		if (hasMaterial(key)){
			auto &_material = m_materialCache[key];
			if (_material != nullptr)return *_material;
		}

		//PRINT("ADDED MATERIAL: " << key);
		auto _materialPtr = std::make_shared<Textures::Material>();
		bool _setMaterialProperties = Textures::loadMaterialConfig(*_materialPtr, key);
		_materialPtr->m_materialName = key; //Set material name

		for (auto &_textureTypePair : std::vector<std::pair<std::string, Textures::TextureType>>{
			{ "albedo", Textures::TEXTURE_ALBEDO },
			{ "normal", Textures::TEXTURE_NORMAL },
			{ "roughness", Textures::TEXTURE_ROUGHNESS },
			{ "metallic", Textures::TEXTURE_METALLIC },
			{ "occlusion", Textures::TEXTURE_OCCLUSION },
			{ "height", Textures::TEXTURE_HEIGHT } })
		{
			//Move one directory back from textures directory
			std::string _texturePath = key + "/" + _textureTypePair.first + ".png";
			m_textureCache[_texturePath] = std::make_shared<Textures::Texture>(
				"../materials/" + _texturePath, true, _textureTypePair.second);

			_materialPtr->setTexture(m_textureCache[_texturePath]);
		}

		m_materialCache[key] = _materialPtr;

		return *m_materialCache[key];
	}

	Textures::Material &ResourceManager::addMaterial(const std::string &key, Textures::Material &&material){
		if (hasMaterial(key)){
			auto &_material = m_materialCache[key];
			if (_material != nullptr)return *_material;
		}

		m_materialCache[key] = std::make_unique<Textures::Material>(std::move(material));

		return *m_materialCache[key];
	}

	void ResourceManager::reloadMaterialConfigs(){
		for (auto &_nameMaterialPair : m_materialCache){
			Textures::loadMaterialConfig(*_nameMaterialPair.second, _nameMaterialPair.first);
		}
		//TOOD update collision bodies
	}



	void ResourceManager::reloadShaders(){
		for (auto &_shader : m_shaderCache){
			//if (_shader.first == "water/water")continue; //TODO REMOVE
			_shader.second->reload();
		}
	}
}