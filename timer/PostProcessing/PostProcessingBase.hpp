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


#ifndef POST_PROCESSING_BASE_HPP
#define POST_PROCESSING_BASE_HPP

#include <memory>

#include "../Buffer/Framebuffer.hpp"


namespace Engine{ namespace Shaders{
	class Shader;
} }


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class PostProcessingBase{
			protected:
				Buffers::Framebuffer m_framebuffer;
				std::shared_ptr<Shaders::Shader> m_shader = nullptr;

				unsigned int m_textureWidth = 0;
				unsigned int m_textureHeight = 0;

				unsigned int m_timerStartQuery = 0;
				unsigned int m_timerEndQuery = 0;

			public:
				PostProcessingBase(std::shared_ptr<Shaders::Shader> shaderPtr,
					unsigned int textureWidth, unsigned int textureHeight, bool hasDepth);

				PostProcessingBase(const PostProcessingBase &object);

				PostProcessingBase(PostProcessingBase &&object);

				virtual ~PostProcessingBase();


				virtual unsigned int useEffect();

				virtual const unsigned int &getResult(const std::size_t &index = 0) const;


				void startTimer();

				void stopTimer();

				double getEffectDuration() const;


				const std::string &getShaderPath() const;

				const unsigned int &getTextureWidth() const;

				const unsigned int &getTextureHeight() const;

			};

		}
	}
}

#endif //POST_PROCESSING_BASE_HPP