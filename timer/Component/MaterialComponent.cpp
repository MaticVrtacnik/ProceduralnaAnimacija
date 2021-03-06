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


#include "MaterialComponent.hpp"

#include "../Texture/Material.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			MaterialComponent::MaterialComponent(){

			}

			MaterialComponent::MaterialComponent(std::shared_ptr<Textures::Material> material) :
				m_material(material)
			{
				//PRINT("MATERIAL: " << material);
			}

			MaterialComponent::MaterialComponent(
				std::shared_ptr<Textures::Material> material, const glm::vec3 &color
			) :
				m_material(material), m_color(color)
			{
			}

			MaterialComponent::MaterialComponent(
				std::shared_ptr<Textures::Material> material, glm::vec3 &&color
			) :
				m_material(material), m_color(std::move(color))
			{
			}

			MaterialComponent::MaterialComponent(
				std::shared_ptr<Textures::Material> material, float r, float g, float b
			) :
				m_material(material), m_color(glm::vec3(r, g, b))
			{
			}

			MaterialComponent::MaterialComponent(const glm::vec3 &color) :
				m_color(color)
			{
			}

			MaterialComponent::MaterialComponent(glm::vec3 &&color) :
				m_color(std::move(color))
			{
			}

			MaterialComponent::MaterialComponent(float r, float g, float b) :
				m_color(glm::vec3(r, g, b))
			{
			}

			MaterialComponent::~MaterialComponent(){

			}


			//MATERIAL
			void MaterialComponent::setMaterial(std::shared_ptr<Textures::Material> material){
				m_material = material;
			}

			std::shared_ptr<Textures::Material> MaterialComponent::getMaterial() const{
				return m_material;
			}


			//ALBEDO
			bool MaterialComponent::hasAlbedoTexture() const{
				return m_material != nullptr ? m_material->hasAlbedo() : false;
			}

			unsigned int MaterialComponent::getAlbedoTexture() const{
				return m_material != nullptr ? m_material->getAlbedoTexture() : 0;
			}


			//NORMAL
			bool MaterialComponent::hasNormalTexture() const{
				return m_material != nullptr ? m_material->hasNormal() : false;
			}

			unsigned int MaterialComponent::getNormalTexture() const{
				return m_material != nullptr ? m_material->getNormalTexture() : 0;
			}


			//ROUGHNESS
			bool MaterialComponent::hasRoughnessTexture() const{
				return m_material != nullptr ? m_material->hasRoughness() : false;
			}

			unsigned int MaterialComponent::getRoughnessTexture() const{
				return m_material != nullptr ? m_material->getRoughnessTexture() : 0;
			}


			//METALLIC
			bool MaterialComponent::hasMetallicTexture() const{
				return m_material != nullptr ? m_material->hasMetallic() : false;
			}

			unsigned int MaterialComponent::getMetallicTexture() const{
				return m_material != nullptr ? m_material->getMetallicTexture() : 0;
			}


			//OCCLUSION
			bool MaterialComponent::hasOcclusionTexture() const{
				return m_material != nullptr ? m_material->hasOcclusion() : false;
			}

			unsigned int MaterialComponent::getOcclusionTexture() const{
				return m_material != nullptr ? m_material->getOcclusionTexture() : 0;
			}


			//HEIGHT
			bool MaterialComponent::hasHeightTexture() const{
				return m_material != nullptr ? m_material->hasHeight() : false;
			}

			unsigned int MaterialComponent::getHeightTexture() const{
				return m_material != nullptr ? m_material->getHeightTexture() : 0;
			}


			//OPACITY
			bool MaterialComponent::hasOpacityTexture() const{
				return m_material != nullptr ? m_material->hasOpacity() : false;
			}

			unsigned int MaterialComponent::getOpacityTexture() const{
				return m_material != nullptr ? m_material->getOpacityTexture() : 0;
			}


			//COLOR
			const glm::vec3 &MaterialComponent::getColor() const{
				return m_color;
			}

			void MaterialComponent::setColor(const glm::vec3 &color){
				m_color = color;
			}

			void MaterialComponent::setColor(glm::vec3 &&color){
				m_color = std::move(color);
			}

			void MaterialComponent::setColor(float r, float g, float b){
				m_color = glm::vec3(r, g, b);
			}


			//COLOR RED
			const float &MaterialComponent::getR() const{
				return m_color.r;
			}

			void MaterialComponent::setColorR(float r){
				m_color.r = r;
			}


			//COLOR GREEN
			const float &MaterialComponent::getG() const{
				return m_color.g;
			}

			void MaterialComponent::setColorG(float g){
				m_color.g = g;
			}


			//COLOR BLUE
			const float &MaterialComponent::getB() const{
				return m_color.b;
			}

			void MaterialComponent::setColorB(float b){
				m_color.b = b;
			}

		}
	}
}
