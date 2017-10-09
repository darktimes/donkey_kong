#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "renderer.hpp"
#include "math.hpp"


namespace Rendering {

  /**
  * Contains information for a character to be dsiplayed properly
  */
  struct FontChar {
		FontChar(GLuint, Math::vec2f, Math::vec2f, unsigned);
		~FontChar() {};
		unsigned textureId;
		Math::vec2f size;
		Math::vec2f bearing;
		unsigned advance;
	};

  class OpenGLShaderProgram;

  class OpenGLTexture;

  /**
  * Implements Renderer with openGL API.
  */
  class OpenGlRenderer: public Renderer {
    private:
    	static unsigned int textVBO, textVAO, textEBO;
      static std::map<std::string, std::unique_ptr<OpenGLShaderProgram>> shaders;
      static std::map<std::string, std::unique_ptr<OpenGLTexture>> textures;
      static std::map<unsigned char, std::unique_ptr<FontChar>> font;
      static std::map<std::string, int> spriteSurfaces;

      void addSpriteSurface(float* vertices, const std::string& name);

    public:
      void init() override;
      void draw() override;
      void terminate() override;
      void createWindow(Math::vec2ui, std::string) override;
      void generateTexture(const std::string&, RawTexture*) override;
      void generateFontChar(unsigned char c, Math::vec2f size,
       Math::vec2f bearing, unsigned advance, std::vector<unsigned char>*) override;
      void drawSprite(SpriteView*) override;
      void drawText(TextView*) override;
      OpenGlRenderer();
      void test(std::string);
  };

  /**
  * Texture relevant data and functionality within this class
  */
  class OpenGLTexture {
    friend class OpenGlRenderer;
  public:
    GLuint id;
    GLuint width, height;

    GLuint internalFormat;
    GLuint imageFormat;

    GLuint wrapS;
    GLuint wrapT;
    GLuint filterMin;
    GLuint filterMax;
  public:
    OpenGLTexture(RawTexture*);
    void bind() const;
  };

  /**
  * Convenient class to stack vertex and fragment shader together and use them.
  */
  class OpenGLShaderProgram {
    public:
      GLuint programHandle;

      OpenGLShaderProgram(const std::string& vertex_shader_file,
        const std::string& fragment_shader_file);
      void use();

      void addUniformFloat(const GLchar *name, float value);
      void addUniformInt	(const GLchar *name, int value);
      void addUniformVec2f(const GLchar *name, const Math::vec2f &value);
      void addUniformVec3f(const GLchar *name, const Math::vec3f &value);
      void addUniformMatrix4(const GLchar *name, const Math::mat4 &matrix);

    private:
      GLuint createShader(GLenum eShaderType, const std::string& file_path);
  };
};
