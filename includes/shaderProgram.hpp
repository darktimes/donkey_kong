#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>
#include "math.hpp"

namespace Renderer {
	
	//std::string* VERTEX_SHADER = new std::string("shader.vert");
	//std::string* FRAGMENT_SHADER = new std::string("shader.frag");
//	enum ERendererType {opengl = 0, vulkan = 1};

	class ShaderProgram {
		public:
			static std::string* SHADER_PATH;
			GLuint programHandle;
		
			ShaderProgram(const std::string& vertex_shader_file, 
				const std::string& fragment_shader_file, 
				const std::string& fragment_geometry_file = std::string());
			void use();

			void addUniformFloat	(const GLchar *name, GLfloat value, GLboolean useShader = false);
			void addUniformInt	(const GLchar *name, GLint value, GLboolean useShader = false);
			void addUniformVec2f	(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
			void addUniformVec2f	(const GLchar *name, const Math::vec2<GLfloat> &value, GLboolean useShader = false);
			void addUniformVec3f	(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
			void addUniformVec3f	(const GLchar *name, const Math::vec3<GLfloat> &value, GLboolean useShader = false);
//			void addUniformVec4f	(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
//			void addUniformVec4f	(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
			void addUniformMatrix4	(const GLchar *name, const Math::mat4 &matrix, GLboolean useShader = false);

			static std::string getFullShaderPath(const std::string& shader_file);
		private: 
			GLuint createShader(GLenum eShaderType, const std::string& file_path);
			std::string readFile(const std::string& path);
	};

}

