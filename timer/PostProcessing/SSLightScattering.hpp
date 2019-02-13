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


#ifndef SS_LIGHT_SCATTERING_HPP
#define SS_LIGHT_SCATTERING_HPP

#include <glm\glm.hpp>

#include "GaussianBlur.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class SSLightScattering : public PostProcessingBase{
			private:
				GaussianBlur m_blur;

			public:
				SSLightScattering(std::shared_ptr<Shaders::Shader> shader, 
					std::shared_ptr<Shaders::Shader> gaussianBlurShader,
					unsigned int width, unsigned int height);

				~SSLightScattering();


				unsigned int useEffect(const glm::mat4 &viewMatrix, 
					const glm::mat4 &projectionMatrix, const glm::vec3 &lightLocation, 
					const glm::vec3 &lightScale, float decay, float density, float weight,
					unsigned int occludedLightTextureId);
			
			};

		}
	}
}

#endif //SS_LIGHT_SCATTERING_HPP