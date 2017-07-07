#include <GL/glew.h>
#include <iostream>
#include <map>
#include <string>

#include "texture2d.hpp"
#include "shaderProgram.hpp"
#include "records.hpp"

namespace Engine {
	struct FontChar {
		FontChar(GLuint, Math::vec2<GLint>, Math::vec2<GLint>, GLuint);
		~FontChar();
		GLuint textureId;
		Math::vec2<GLint> size;
		Math::vec2<GLint> bearing;
		GLuint advance;
	};

	class ResourceManager
	{
		public:
			static std::map<std::string, Renderer::ShaderProgram*> shaders;
			static std::map<std::string, Renderer::Texture2D*> textures;
			static std::map<GLchar, FontChar*> font;
			static RecordTable* recordTable;

			static void init();
			static Renderer::ShaderProgram* getShaderProgram(std::string name);
			static Renderer::Texture2D* getTexture(std::string name);

			static void loadShader(const std::string& vertex_shader_file, 
				const std::string& fragment_shader_file, 
				const std::string& fragment_geometry_file, std::string name);
			static void loadTexture(const std::string&, GLboolean alpha, std::string name);
			static void loadFont(std::string fontName);
			static void clear();
		private:
			ResourceManager() {}
//			static Renderer::Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
	};
}
