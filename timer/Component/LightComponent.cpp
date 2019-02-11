#include "LightComponent.hpp"

#include "../Utility/MathUtilities.hpp"
#include "../Shader/Shader.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			LightComponent::LightComponent(){

			}

			LightComponent::LightComponent(LightType type){
				setType(type);
			}

			//POINT LIGHT
			LightComponent::LightComponent(const glm::vec3 &color, 
				float intensity, float attenuation, float shininess)
			{
				m_color = color;
				m_intensity = intensity;
				m_quadricAttenuation = attenuation;
				m_linearAttenuation = attenuation;
				setType(LIGHT_POINT);
			}

			//POINT LIGHT
			LightComponent::LightComponent(glm::vec3 &&color, 
				float intensity, float attenuation, float shininess)
			{
				m_color = std::move(color);
				m_intensity = intensity;
				m_quadricAttenuation = attenuation;
				m_linearAttenuation = attenuation;
				setType(LIGHT_POINT);
			}

			//DIRECTIONAL LIGHT
			LightComponent::LightComponent(const glm::vec3 &color, float intensity, float shininess){
				m_color = color;
				m_intensity = intensity;
				setType(LIGHT_DIRECTIONAL);
			}

			//DIRECTIONAL LIGHT
			LightComponent::LightComponent(glm::vec3 &&color, float intensity, float shininess){
				m_color = std::move(color);
				m_intensity = intensity;
				setType(LIGHT_DIRECTIONAL);
			}

			//SPOTLIGHT
			LightComponent::LightComponent(const glm::vec3 &direction, const glm::vec3 &color,
				float intensity, float coneAngle, float shininess)
			{
				m_color = color;
				m_direction = direction;
				m_intensity = intensity;
				m_coneAngle = coneAngle;
				setType(LIGHT_SPOTLIGHT);
			}

			//SPOTLIGHT
			LightComponent::LightComponent(glm::vec3 &&direction, glm::vec3 &&color,
				float intensity, float coneAngle, float shininess)
			{
				m_color = std::move(color);
				m_direction = std::move(direction);

				m_intensity = intensity;
				m_coneAngle = coneAngle;
				setType(LIGHT_SPOTLIGHT);
			}



			void LightComponent::setUniforms(int index, 
				const glm::vec3 &location, Shaders::Shader &shader)
			{
				if (index >= c_maxLights) return;

				float _w = m_type == LIGHT_DIRECTIONAL ? 0.0f : 1.0f;
				//PRINT("W: " << _w)
				std::string _struct = "u_lights[" + std::to_string(index) + "].";

				shader.setUniform((_struct + "linearAttenuation").c_str(), m_linearAttenuation);
				shader.setUniform((_struct + "quadricAttenuation").c_str(), m_quadricAttenuation);
				shader.setUniform((_struct + "intensity").c_str(), m_intensity);
				shader.setUniform((_struct + "coneAngle").c_str(), m_coneAngle / 2.0f);
				shader.setUniform((_struct + "color").c_str(), m_color);
				shader.setUniform((_struct + "direction").c_str(), glm::normalize(m_direction));
				shader.setUniform((_struct + "location").c_str(), glm::vec4(location, _w));
			}



			const glm::vec3 &LightComponent::getDirection() const{
				return m_direction;
			}

			void LightComponent::setDirection(const glm::vec3 &direction, bool normalize){
				m_direction = normalize ? glm::normalize(direction) : direction;
			}

			void LightComponent::setDirection(glm::vec3 &&direction, bool normalize){
				m_direction = normalize ? glm::normalize(std::move(direction)) : std::move(direction);
			}

			void LightComponent::setDirection(const glm::quat &quaternion){
				m_direction = Utilities::getVectorFromRotation(quaternion);
			}

			void LightComponent::setDirection(glm::quat &&quaternion){
				m_direction = Utilities::getVectorFromRotation(std::move(quaternion));
			}

			

			LightType LightComponent::getType() const{
				return m_type;
			}

			void LightComponent::setType(LightType type){
				if (m_type == type)return;

				m_type = type;
				if (m_type == LIGHT_SPOTLIGHT){
					m_coneAngle = 30.0f;
				}
			}

		

			void LightComponent::setConeAngle(float coneAngle){
				m_coneAngle = coneAngle;
			}

			float LightComponent::getConeAngle() const{
				return m_coneAngle;
			}

		}
	}
}
