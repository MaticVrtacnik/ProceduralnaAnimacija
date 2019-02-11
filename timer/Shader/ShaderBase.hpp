#ifndef SHADER_BASE_HPP
#define SHADER_BASE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <GL\glew.h>
#include <glm\glm.hpp>


namespace Engine{
	namespace Shaders{

		static const std::string c_ShadersDirectory = "data/shaders/";
		static const std::string c_GLSLVersion = "#version 430 core \n";
		static const std::string c_VSformat = "vs";
		static const std::string c_FSformat = "frag";
		static const std::string c_GSformat = "gs";
		static const std::string c_CSformat = "compute";
		static const std::string c_TCSformat = "tcs";
		static const std::string c_TESformat = "tes";

		enum ShaderType{
			SHADER_INVALID,
			SHADER_DEFAULT,
			SHADER_GEOMETRY,
			SHADER_TESSELLATION,
			SHADER_COMPUTE,
			SHADER_TYPE_COUNT
		};


		class ShaderBase{
		protected:
			std::string m_name;
			std::string m_defines;
			unsigned int m_program;

			//ShaderType m_type;
			std::unordered_map<std::string, GLint> m_uniformLocations;


			std::string loadSourceFromFile(const std::string &path, 
				const std::string &includedIn = "");

			unsigned int compileShader(const std::string &source, unsigned int mode);

			void printErrors(unsigned int shaderId, const std::string &path);

			unsigned int loadShader(const std::string &format, unsigned int mode);

		public:
			ShaderBase();

			~ShaderBase();
	

			virtual void init(const std::string &name, 
				const std::vector<std::string> &defines = {});

			virtual void destroy();

			void reload();

			void bind() const;

			void unbind() const;

			const unsigned int &getProgram() const;
			//const ShaderType &getType() const;

			void setUniform(const GLchar *name, int v);
			void setUniform(const GLchar *name, int v0, int v1);
			void setUniform(const GLchar *name, int v0, int v1, int v2);
			void setUniform(const GLchar *name, int v0, int v1, int v2, int v3);

			void setUniform(const GLchar *name, float v);
			void setUniform(const GLchar *name, float v0, float v1);
			void setUniform(const GLchar *name, float v0, float v1, float v2);
			void setUniform(const GLchar *name, float v0, float v1, float v2, float v3);

			void setUniform(const GLchar *name, const glm::vec2 &v);
			void setUniform(const GLchar *name, const glm::ivec2 &v);
			void setUniform(const GLchar *name, const glm::bvec2 &v);

			void setUniform(const GLchar *name, const glm::vec3 &v);
			void setUniform(const GLchar *name, const glm::ivec3 &v);
			void setUniform(const GLchar *name, const glm::bvec3 &v);

			void setUniform(const GLchar *name, const glm::vec4 &v);
			void setUniform(const GLchar *name, const glm::ivec4 &v);
			void setUniform(const GLchar *name, const glm::bvec4 &v);

			void setUniform(const GLchar *name, const glm::mat2 &v);
			void setUniform(const GLchar *name, const glm::mat3 &v);
			void setUniform(const GLchar *name, const glm::mat4 &v);

		};

	}
}

#endif //SHADER_BASE_HPP