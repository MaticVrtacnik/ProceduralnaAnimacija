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


#ifndef SAO_HPP
#define SAO_HPP

#include <glm\glm.hpp>

#include "SAOBlur.hpp"
#include "GaussianBlur.hpp"
#include "TextureSizeChanger.hpp"
#include "LinearizeDepth.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{


			class SAO : public PostProcessingBase{
			private:
				static const unsigned int NUM_MIPMAPS = 5;

				SAOBlur m_blur;
				TextureSizeChanger m_textureSizeChanger;
				LinearizeDepth m_linearizeDepth;

				std::vector<unsigned int> m_depthTextureMipmaps;
				//GaussianBlur m_gaussianBlur;

			public:
				SAO(std::shared_ptr<Shaders::Shader> shader,
					std::shared_ptr<Shaders::Shader> blurShader,
					std::shared_ptr<Shaders::Shader> linearDepthShader,
					std::shared_ptr<Shaders::Shader> sizeChangerShader,
					unsigned int width, unsigned int height);

				~SAO();


				unsigned int useEffect(const glm::mat4 &projectionMatrix, 
					unsigned int depthTextureId);

				const unsigned int &getResult(const std::size_t &index = 0) const override;

			};

		}
	}
}

#endif //SAO_HPP