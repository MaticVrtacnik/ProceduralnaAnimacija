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


#ifndef TEXTURE_SIZE_CHANGER_HPP
#define TEXTURE_SIZE_CHANGER_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class TextureSizeChanger : public PostProcessingBase{
			private:
				static const unsigned int MAX_MIPMAP_LEVEL = 7;		
				std::vector<unsigned int> m_textureMipmaps;

			public:
				TextureSizeChanger(std::shared_ptr<Shaders::Shader> shader,
					unsigned int width, unsigned int height);

				~TextureSizeChanger();


				unsigned int useEffect(unsigned int textureId, unsigned int mipmapLevel);
			
			};

		}
	}
}

#endif //TEXTURE_SIZE_CHANGER_HPP