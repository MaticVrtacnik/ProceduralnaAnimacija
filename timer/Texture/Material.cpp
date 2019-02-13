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


#include "Material.hpp"

#include "Texture.hpp"
#include "../WorldLoader/pugixml.hpp"
#include "../Define/Assert.hpp"
#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Textures{

		Material::Material(){

		}

		Material::Material(const Material &material) :
			m_albedo(material.m_albedo), m_normal(material.m_normal),
			m_roughness(material.m_roughness), m_metallic(material.m_metallic),
			m_occlusion(material.m_occlusion), m_opacity(material.m_opacity)
		{
		}

		Material::Material(Material &&material) :
			m_albedo(std::move(material.m_albedo)),
			m_normal(std::move(material.m_normal)),
			m_roughness(std::move(material.m_roughness)),
			m_metallic(std::move(material.m_metallic)),
			m_occlusion(std::move(material.m_occlusion)),
			m_opacity(std::move(material.m_opacity))
		{
		}

		Material::Material(
			std::shared_ptr<Texture> albedo,
			std::shared_ptr<Texture> normal,
			std::shared_ptr<Texture> roughness,
			std::shared_ptr<Texture> metallic,
			std::shared_ptr<Texture> occlusion,
			std::shared_ptr<Texture> height,
			std::shared_ptr<Texture> opacity
		) :
			m_albedo(albedo), m_normal(normal), m_roughness(roughness),
			m_metallic(metallic), m_occlusion(occlusion), m_height(height),
			m_opacity(opacity)
		{
		}

		Material::~Material(){

		}



		void Material::setTexture(const std::string &path, TextureType type){
			setTexture(std::make_shared<Texture>(path, true, type));
		}

		void Material::setTexture(std::shared_ptr<Texture> texture){
			ASSERT(texture != nullptr, "Texture is a null pointer");

			switch (texture->m_type){
			case TEXTURE_ALBEDO:	m_albedo = texture; break;
			case TEXTURE_NORMAL:	m_normal = texture; break;
			case TEXTURE_ROUGHNESS:	m_roughness = texture; break;
			case TEXTURE_METALLIC:	m_metallic = texture; break;
			case TEXTURE_OCCLUSION:	m_occlusion = texture; break;
			case TEXTURE_HEIGHT:	m_height = texture; break;
			case TEXTURE_OPACITY:	m_opacity = texture; break;

			default: 		
				WARNING("Uknown material texture type '" + 
					std::to_string(static_cast<int>(texture->m_type)) + "'");
				break;
			}
		}

		void Material::setTextureId(unsigned int textureId, TextureType type){
			/*switch (type){
				case TEXTURE_ALBEDO:	if(hasAlbedo())m_albedo->m_textureId = textureId;
				case TEXTURE_NORMAL:	if(hasNormal())m_normal->m_textureId = textureId;
				case TEXTURE_ROUGHNESS:	if (hasRoughness())m_roughness->m_textureId = textureId;
				case TEXTURE_METALLIC:	if (hasMetallic())m_metallic->m_textureId = textureId;
				case TEXTURE_OCCLUSION:	if (hasOcclusion())m_occlusion->m_textureId = textureId;
				case TEXTURE_HEIGHT:	if (hasHeight())m_height->m_textureId = textureId;
				case TEXTURE_OPACITY:	if (hasOpacity())m_opacity->m_textureId = textureId;
				default: ASSERT(0, "Unknown texture type in Material::setTextureId");
			}*/

			//TODO maybe - error with full optimizations
			auto _texture = &getTexture(type);

			if (_texture->m_textureId != 0){
				//TODO maybe delete 
			}

			_texture->m_textureId = textureId;
		}


		Texture &Material::getTexture(TextureType type) const{
			//TODO check if textures are valid

			switch (type){
				case TEXTURE_ALBEDO:	return *m_albedo;
				case TEXTURE_NORMAL:	return *m_normal;
				case TEXTURE_ROUGHNESS:	return *m_roughness;
				case TEXTURE_METALLIC:	return *m_metallic;
				case TEXTURE_OCCLUSION:	return *m_occlusion;
				case TEXTURE_HEIGHT:	return *m_height;
				case TEXTURE_OPACITY:	return *m_opacity;
				default: ASSERT(0, "Unknown texture type in Material::getTexture");
			}
		}

		bool Material::hasTexture(TextureType type) const{
			switch (type){
				case TEXTURE_ALBEDO:	return hasAlbedo();
				case TEXTURE_NORMAL:	return hasNormal(); 
				case TEXTURE_ROUGHNESS:	return hasRoughness();
				case TEXTURE_METALLIC:	return hasMetallic(); 
				case TEXTURE_OCCLUSION:	return hasOcclusion();
				case TEXTURE_HEIGHT:	return hasHeight();
				case TEXTURE_OPACITY:	return hasOpacity();
				default: return false;
			}

			return false;
		}

		bool Material::hasAlbedo() const{
			return m_albedo != nullptr;
		}

		unsigned int Material::getAlbedoTexture() const{
			return hasAlbedo() ? m_albedo->m_textureId : 0;
		}

		bool Material::hasNormal() const{
			return m_normal != nullptr;
		}

		unsigned int Material::getNormalTexture() const{
			return hasNormal() ? m_normal->m_textureId : 0;
		}

		bool Material::hasRoughness() const{
			return m_roughness != nullptr;
		}

		unsigned int Material::getRoughnessTexture() const{
			return hasRoughness() ? m_roughness->m_textureId : 0;
		}

		bool Material::hasMetallic() const{
			return m_metallic != nullptr;
		}

		unsigned int Material::getMetallicTexture() const{
			return hasMetallic() ? m_metallic->m_textureId : 0;
		}

		bool Material::hasOcclusion() const{
			return m_occlusion != nullptr;
		}

		unsigned int Material::getOcclusionTexture() const{
			return hasOcclusion() ? m_occlusion->m_textureId : 0;
		}

		bool Material::hasHeight() const{
			return m_height != nullptr;
		}

		unsigned int Material::getHeightTexture() const{
			return hasHeight() ? m_height->m_textureId : 0;
		}

		bool Material::hasOpacity() const{
			return m_opacity != nullptr;
		}

		unsigned int Material::getOpacityTexture() const{
			return hasOpacity() ? m_opacity->m_textureId : 0;
		}



		bool loadMaterialConfig(Material &material, const std::string &materialName){
			std::string _materialConfigPath = "data/materials/" + materialName + "/config.mat";

			auto f_wrongFormat = [&_materialConfigPath](const std::string &type){
				PRINT("Error loading material config file '" << _materialConfigPath << "': " << type);
			};


			pugi::xml_document _file;
			if (!_file.load_file(_materialConfigPath.c_str())){
				ASSERT(0, "Material configuration file '" + _xmlPath + "' not found or not valid");
				f_wrongFormat("File not found or not valid");
				return false;
			}

			pugi::xml_node _rootNode = _file.child("Material");
			if (!_rootNode){
				f_wrongFormat("Invalid root node");
				return false;
			}

			//for (auto _child = _rootNode.first_child(); _child; _child = _child.next_sibling())

			auto _physicsSettings = _rootNode.child("Physics");
			if (_physicsSettings){
				auto _frictionAttribute = _physicsSettings.attribute("friction");
				if (_frictionAttribute){
					material.m_friction = _frictionAttribute.as_float();
				}

				auto _restitutionAttribute = _physicsSettings.attribute("restitution");
				if (_restitutionAttribute){
					material.m_restitution = _restitutionAttribute.as_float();
				}
			}

			auto _displacementSettings = _rootNode.child("Displacement");
			if (_displacementSettings){
				auto _scaleAttribute = _displacementSettings.attribute("scale");
				if (_scaleAttribute){
					material.m_displacementScale = _scaleAttribute.as_float();
					PRINT("MATERIAL SCALE: " << material.m_displacementScale);
				}
			}

			auto _soundSettings = _rootNode.child("Sound");
			if (_soundSettings){

			}

			return true;
		}

	}
}
