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