#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glm\gtx\transform.hpp>

#include "../Buffer/ArrayBuffer.hpp"
#include "../Shader/Shader.hpp"

namespace Engine{
	namespace Environment{

		class Skybox{
		private:
			std::vector<unsigned int> m_cubemaps;
			int m_selectedIndex = 0;

			Buffers::ArrayBuffer m_vertexBuffer;

		public:
			unsigned int m_vertexArray = 0;

		public:
			Skybox();

			~Skybox();


			const std::vector<unsigned int> &getCubemaps() const;

			const unsigned int &getCubemap(std::size_t index) const;


			const unsigned int &getSelectedCubemap() const;

			//void setCubemap(unsigned int cubemap);

			void setNextCubemap();

			void setPreviousCubemap();

		};

	}
}

#endif //SKYBOX_HPP