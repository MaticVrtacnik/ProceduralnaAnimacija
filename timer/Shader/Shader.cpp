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
