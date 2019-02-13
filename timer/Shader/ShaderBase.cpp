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


#include "ShaderBase.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include "../Define/PrintDefines.hpp"

namespace Engine{
	namespace Shaders{

		ShaderBase::ShaderBase() :
			m_name(""), 
			m_defines("")
		{
		}

		ShaderBase::~ShaderBase(){
			destroy();
		}


		std::string ShaderBase::loadSourceFromFile(
			const std::string &path, const std::string &includedIn)
		{
			std::ifstream _fileInputStream(path);
			if (!_fileInputStream.is_open()){
				std::string _includeString = includedIn.empty() ?
					"" : (includedIn + ": ");
				ERROR(_includeString + "Unable to load shader " + path);
				return "";
			}

			std::string _source;
			while (!_fileInputStream.eof())
			{
				std::string _line;
				std::getline(_fileInputStream, _line);

				std::stringstream _stringStream(_line);
				std::string _head;
				_stringStream >> _head;

				//Recursion
				if (_head == "#include"){
					_stringStream >> _head;
					_head = _head.substr(1, _head.length() - 2);
					_source += this->loadSourceFromFile("data/shaders/" + _head, path) + "\n";
				}
				else{
					_source += _line + "\n";
				}

			}

			return _source;
		}

		unsigned int ShaderBase::compileShader(const std::string &source, unsigned int mode){
			unsigned int _shaderId = glCreateShader(mode);
			const char *_cSource = source.c_str();

			glShaderSource(_shaderId, 1, &_cSource, NULL);
			glCompileShader(_shaderId);

			return _shaderId;
		}

		void ShaderBase::printErrors(unsigned int shaderId, const std::string &path){
			char error[1000];
			glGetShaderInfoLog(shaderId, 1000, NULL, error);

			if (error[0]){
				std::cout << path << " " << error << std::endl;
			}
		}

		unsigned int ShaderBase::loadShader(const std::string &format, unsigned int mode){
			std::string _source = c_GLSLVersion + m_defines;

			const std::string _path(m_name + "." + format);
			_source += loadSourceFromFile(c_ShadersDirectory + _path);

			unsigned int _shaderId = compileShader(_source, mode);
			printErrors(_shaderId, _path);

			return _shaderId;
		}


		void ShaderBase::bind() const{
			glUseProgram(m_program);
		}

		void ShaderBase::unbind() const{
			glUseProgram(0);
		}

		const unsigned int &ShaderBase::getProgram() const{
			return m_program;
		}

		/*const ShaderType &ShaderBase::getType() const{
			return m_type;
		}*/


		void ShaderBase::init(const std::string &name,
			const std::vector<std::string> &defines)
		{
			m_name = name;

			for (auto &_define : defines){
				if (!_define.empty()){
					m_defines += "#define " + _define + "\n";
				}
			}		
		}

		void ShaderBase::destroy(){		
			glDeleteProgram(m_program);
			m_uniformLocations.clear();
		}

		void ShaderBase::reload(){
			destroy();
			init(m_name);

			//TODO set all uniforms
			/*bind();
			for (std::size_t i = 0; i < m_uniformLocations.size(); ++i){
			glUniform1i(m_uniformLocations[i], m_uniformValues[i]);
			}
			unbind();*/
		}


		void ShaderBase::setUniform(const GLchar *name, int v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform1i(m_uniformLocations[name], v);
		}

		void ShaderBase::setUniform(const GLchar *name, int v0, int v1){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform2i(m_uniformLocations[name], v0, v1);
		}

		void ShaderBase::setUniform(const GLchar *name, int v0, int v1, int v2){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform3i(m_uniformLocations[name], v0, v1, v2);
		}

		void ShaderBase::setUniform(const GLchar *name, int v0, int v1, int v2, int v3){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform4i(m_uniformLocations[name], v0, v1, v2, v3);
		}

		void ShaderBase::setUniform(const GLchar *name, float v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform1f(m_uniformLocations[name], v);
		}

		void ShaderBase::setUniform(const GLchar *name, float v0, float v1){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform2f(m_uniformLocations[name], v0, v1);
		}

		void ShaderBase::setUniform(const GLchar *name, float v0, float v1, float v2){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform3f(m_uniformLocations[name], v0, v1, v2);
		}

		void ShaderBase::setUniform(const GLchar *name, float v0, float v1, float v2, float v3){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform4f(m_uniformLocations[name], v0, v1, v2, v3);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::vec2 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform2f(m_uniformLocations[name], v.x, v.y);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::ivec2 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform2i(m_uniformLocations[name], v.x, v.y);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::bvec2 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform2i(m_uniformLocations[name], v.x, v.y);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::vec3 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform3f(m_uniformLocations[name], v.x, v.y, v.z);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::ivec3 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform3i(m_uniformLocations[name], v.x, v.y, v.z);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::bvec3 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform3i(m_uniformLocations[name], v.x, v.y, v.z);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::vec4 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform4f(m_uniformLocations[name], v.x, v.y, v.z, v.w);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::ivec4 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform4i(m_uniformLocations[name], v.x, v.y, v.z, v.w);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::bvec4 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniform4i(m_uniformLocations[name], v.x, v.y, v.z, v.w);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::mat2 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniformMatrix2fv(m_uniformLocations[name], 1, GL_FALSE, &v[0][0]);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::mat3 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniformMatrix3fv(m_uniformLocations[name], 1, GL_FALSE, &v[0][0]);
		}

		void ShaderBase::setUniform(const GLchar *name, const glm::mat4 &v){
			if (m_uniformLocations.find(name) == m_uniformLocations.end()){
				m_uniformLocations[name] = glGetUniformLocation(m_program, name);
			}glUniformMatrix4fv(m_uniformLocations[name], 1, GL_FALSE, &v[0][0]);
		}
	
		//TODO convert to iterators to save lookup 

	}
}
