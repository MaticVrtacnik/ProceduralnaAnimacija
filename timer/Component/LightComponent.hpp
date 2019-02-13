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


#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

#include "Component.hpp"


namespace Engine{ namespace Shaders{
	class Shader;
} }


namespace Engine{
	namespace Entities{
		namespace Components{

			enum LightType{
				LIGHT_POINT			= 0,
				LIGHT_SPOTLIGHT		= 1,
				LIGHT_DIRECTIONAL	= 2
			};


			class LightComponent : public Component{
			public:
				static const unsigned int c_maxLights = 10;

			public:
				float m_intensity = 3.0f;
				float m_quadricAttenuation = 0.1f;
				float m_linearAttenuation = 0.1f;		
				glm::vec3 m_color = glm::vec3(1.0f, 0.85f, 0.6f);

			private:
				LightType m_type = LIGHT_POINT;
				glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
				float m_coneAngle = 0.0f;

			public:
				LightComponent();

				LightComponent(LightType type);

				//POINT LIGHT
				LightComponent(const glm::vec3 &color, float intensity, float attenuation, float shininess);

				//POINT LIGHT
				LightComponent(glm::vec3 &&color, float intensity, float attenuation, float shininess);

				//DIRECTIONAL LIGHT
				LightComponent(const glm::vec3 &color, float intensity, float shininess);

				//DIRECTIONAL LIGHT
				LightComponent(glm::vec3 &&color, float intensity, float shininess);

				//SPOTLIGHT
				LightComponent(const glm::vec3 &direction, const glm::vec3 &color,
					float intensity, float coneAngle, float shininess);

				//SPOTLIGHT
				LightComponent(glm::vec3 &&direction, glm::vec3 &&color,
					float intensity, float coneAngle, float shininess);


				void setUniforms(int index, const glm::vec3 &location, Shaders::Shader &shader);


				const glm::vec3 &getDirection() const;

				void setDirection(const glm::vec3 &direction, bool normalize = true);

				void setDirection(glm::vec3 &&direction, bool normalize = true);

				void setDirection(const glm::quat &quaternion);

				void setDirection(glm::quat &&quaternion);


				LightType getType() const;

				void setType(LightType type);


				float getConeAngle() const;

				void setConeAngle(float coneAngle);

			};


		}
	}
}

#endif //LIGHT_COMPONENT_HPP