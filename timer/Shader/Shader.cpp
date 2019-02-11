#include "Shader.hpp"


namespace Engine{
	namespace Shaders{

		Shader::Shader(){

		}

		Shader::~Shader(){

		}


		void Shader::destroy(){
			glDetachShader(m_program, m_vertexShader);
			glDeleteShader(m_vertexShader);

			glDetachShader(m_program, m_fragmentShader);
			glDeleteShader(m_fragmentShader);

			ShaderBase::destroy();
		}

		void Shader::init(const std::string &name, const std::vector<std::string> &defines){
			ShaderBase::init(name, defines);

			m_vertexShader = loadShader(c_VSformat, GL_VERTEX_SHADER);
			m_fragmentShader = loadShader(c_FSformat, GL_FRAGMENT_SHADER);

			m_program = glCreateProgram();
			glAttachShader(m_program, m_vertexShader);
			glAttachShader(m_program, m_fragmentShader);

			glLinkProgram(m_program);

			//glDetachShader(m_program, m_vertexShader);
			//glDetachShader(m_program, m_fragmentShader);
		}

	}
}
