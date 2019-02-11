#ifndef SHADER_HPP
#define SHADER_HPP

#include "ShaderBase.hpp"


namespace Engine{
	namespace Shaders{

		class Shader : public ShaderBase{
		protected:
			unsigned int m_vertexShader = 0;
			unsigned int m_fragmentShader = 0;
		
		public:
			Shader();

			~Shader();


			virtual void destroy() override;

			void init(const std::string &name,
				const std::vector<std::string> &defines = {}) override;
			
		};

	}
}

#endif //SHADER_HPP