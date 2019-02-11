#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <vector>


namespace Engine{
	namespace Buffers{

		class Framebuffer{
		private:
			bool m_hasDepth = false;
			unsigned int m_id = 0;
			unsigned int m_depthTexture = 0;
			std::vector<unsigned int> m_textures;

		public:
			Framebuffer();

			~Framebuffer();


			void bind();

			void unbind();


			void init();

			void init(int width, int height, bool hasDepth = true, bool hasStencil = false);

			bool checkErrors();


			void addTexture(unsigned int texture);

			const unsigned int &getTexture(std::size_t id = 0) const;

			const std::vector<unsigned int> &getTextures() const;

			void setTexture(unsigned int texture, std::size_t id = 0);


			const unsigned int &getDepthTexture() const;

			void setDepthTexture(unsigned int texture, bool deleteOld = false);

			bool hasDepth() const;


			//if num == 0 then all attachments will be set
			void setDrawBuffers(std::size_t num = 0);

			void setDrawBuffers(const std::vector<unsigned int> &buffers);

		};

	}
}

#endif //FRAMEBUFFER_HPP