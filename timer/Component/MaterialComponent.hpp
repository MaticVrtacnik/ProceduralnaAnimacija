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


#ifndef MATERIAL_COMPONENT_HPP
#define MATERIAL_COMPONENT_HPP

#include <memory>
#include <glm\glm.hpp>

#include "Component.hpp"


namespace Engine{ namespace Textures{
	struct Material;
} }


namespace Engine{
	namespace Entities{
		namespace Components{

			class MaterialComponent : public Component{
			private:
				glm::vec3 m_color = glm::vec3(0.0f);
				std::shared_ptr<Textures::Material> m_material = nullptr;

			public:
				MaterialComponent();

				MaterialComponent(std::shared_ptr<Textures::Material> material);

				MaterialComponent(std::shared_ptr<Textures::Material> material, const glm::vec3 &color);

				MaterialComponent(std::shared_ptr<Textures::Material> material, glm::vec3 &&color);

				MaterialComponent(std::shared_ptr<Textures::Material> material, float r, float g, float b);

				MaterialComponent(const glm::vec3 &color);

				MaterialComponent(glm::vec3 &&color);

				MaterialComponent(float r, float g, float b);

				~MaterialComponent();


				void setMaterial(std::shared_ptr<Textures::Material> material);

				std::shared_ptr<Textures::Material> getMaterial() const;


				bool hasAlbedoTexture() const;

				unsigned int getAlbedoTexture() const;


				bool hasNormalTexture() const;

				unsigned int getNormalTexture() const;


				bool hasRoughnessTexture() const;

				unsigned int getRoughnessTexture() const;


				bool hasMetallicTexture() const;

				unsigned int getMetallicTexture() const;


				bool hasOcclusionTexture() const;

				unsigned int getOcclusionTexture() const;


				bool hasHeightTexture() const;

				unsigned int getHeightTexture() const;


				bool hasOpacityTexture() const;

				unsigned int getOpacityTexture() const;


				const glm::vec3 &getColor() const;

				void setColor(const glm::vec3 &color);

				void setColor(glm::vec3 &&color);

				void setColor(float r, float g, float b);


				const float &getR() const;

				void setColorR(float r);


				const float &getG() const;

				void setColorG(float g);


				const float &getB() const;

				void setColorB(float b);

			};

		}
	}
}

#endif //MATERIAL_COMPONENT_HPP