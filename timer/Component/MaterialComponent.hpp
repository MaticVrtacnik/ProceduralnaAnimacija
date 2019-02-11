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