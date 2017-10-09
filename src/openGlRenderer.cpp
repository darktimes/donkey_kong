#include <memory>
#include "openGlRenderer.hpp"
#include "utils.hpp"

using namespace Rendering;
using namespace Math;
using namespace Utils;

unsigned int OpenGlRenderer::textVBO, OpenGlRenderer::textVAO, OpenGlRenderer::textEBO;
std::map<std::string, std::unique_ptr<OpenGLTexture>> OpenGlRenderer::textures;
std::map<std::string, std::unique_ptr<OpenGLShaderProgram>> OpenGlRenderer::shaders;
std::map<std::string, int> OpenGlRenderer::spriteSurfaces;
std::map<unsigned char, std::unique_ptr<FontChar>> OpenGlRenderer::font;

FontChar::FontChar(GLuint textureId, vec2f size, vec2f bearing, unsigned advance):
 textureId(textureId), size(size), bearing(bearing), advance(advance) {

}

OpenGlRenderer::OpenGlRenderer() {

}

void OpenGlRenderer::createWindow(vec2ui size, std::string window_title) {

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = std::unique_ptr<Window>(new Window(size, window_title));

  glfwMakeContextCurrent(window->windowHandle);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to init GLEW");
	}

	glViewport(0, 0, size.x, size.y);
	Logger::i("Init-ed window for renderer.");
}

void OpenGlRenderer::init() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  shaders["projectionShader"] =
   std::unique_ptr<OpenGLShaderProgram>(
   new OpenGLShaderProgram(std::string("shader.vert"),
    std::string("shader.frag")));
  shaders["projectionShader"]->use();
  shaders["projectionShader"]->addUniformInt("texture1", 0);
  shaders["projectionShader"]->addUniformMatrix4("projection",
   Math::ortho(0.0f, window->size.x, 0.0f, window->size.y, -1.0f, 1.0f)
  );
  shaders["projectionShader"]->addUniformFloat("maxHeight", GameObject::blockEdgeLength);

  shaders["textShader"] =
   std::unique_ptr<OpenGLShaderProgram>(
   new OpenGLShaderProgram(std::string("textShader.vert"), std::string("textShader.frag")));
   shaders["textShader"]->use();
  shaders["textShader"]->addUniformMatrix4("projection",
    Math::ortho(0.0f, window->size.x, 0.0f, window->size.y, -1.0f, 1.0f)
  );

  float quadratVertices[] = {
  // positions   //texture coords
  	1.0f,  1.0f,   1.0f, 1.0f, // top right
  	1.0f,  0.0f,   1.0f, 0.0f, // bottom rightunsigned
  	0.0f,  0.0f,   0.0f, 0.0f, // bottom left
  	0.0f,  1.0f,   0.0f, 1.0f  // top left
  };

  addSpriteSurface(quadratVertices, "quadratSprite");

  glGenVertexArrays(1, &textVAO);
  glGenBuffers(1, &textVBO);
  glBindVertexArray(textVAO);
  glBindBuffer(GL_ARRAY_BUFFER, textVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void OpenGlRenderer::draw() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto view: drawList) {
    view->draw();
  }
  glfwSwapBuffers(window->windowHandle);
}

void OpenGlRenderer::drawSprite(SpriteView* spriteView) {
  glActiveTexture(GL_TEXTURE0);
  textures[spriteView->getCurrentTextureName()]->bind();
  Math::mat4 model = spriteView->getModelMatrix();
  shaders["projectionShader"]->use();
  shaders["projectionShader"]->addUniformMatrix4("model", model);
  float scaleX = spriteView->parent->dimensions.x / GameObject::blockEdgeLength;
  if (scaleX < 1.0f) scaleX = 1.0f;
  float scaleY = spriteView->parent->dimensions.y / GameObject::blockEdgeLength;
  if (scaleY < 1.0f) scaleY = 1.0f;

  shaders["projectionShader"]->addUniformVec2f("scale", Math::vec2f(scaleX, scaleY));

  shaders["projectionShader"]->addUniformFloat("scaleY", scaleY);
  glBindVertexArray(spriteSurfaces["quadratSprite"]);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void OpenGlRenderer::drawText(TextView* textView) {
  shaders["textShader"]->use();
  shaders["textShader"]->addUniformVec3f("textColor",
   textView->color);
   glActiveTexture(GL_TEXTURE0);
   glBindVertexArray(textVAO);

   // Iterate through all characters
   std::string::const_iterator c;
   GLfloat x = textView->position.x;
   for (c = textView->text.begin(); c != textView->text.end(); c++)
   {
       const std::unique_ptr<FontChar>& ch = font[*c];

       GLfloat xpos = x + ch->bearing.x * textView->scale;
       GLfloat ypos = textView->position.y
        - (ch->size.y - ch->bearing.y) * textView->scale;

       GLfloat w = ch->size.x * textView->scale;
       GLfloat h = ch->size.y * textView->scale;
       //Logger::w("g_w:" + std::to_string(w) + ", g_h:" + std::to_string(h));
       // Update VBO for each character
       GLfloat vertices[6][4] = {
           { xpos,     ypos + h,   0.0, 0.0 },
           { xpos,     ypos,       0.0, 1.0 },
           { xpos + w, ypos,       1.0, 1.0 },

           { xpos,     ypos + h,   0.0, 0.0 },
           { xpos + w, ypos,       1.0, 1.0 },
           { xpos + w, ypos + h,   1.0, 0.0 }
       };
       // Render glyph texture over quad
       glBindTexture(GL_TEXTURE_2D, ch->textureId);
       // Update content of VBO memory
       glBindBuffer(GL_ARRAY_BUFFER, textVBO);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

       glBindBuffer(GL_ARRAY_BUFFER, 0);
       // Render quad
       glDrawArrays(GL_TRIANGLES, 0, 6);
       // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
       x += (ch->advance>>6) * textView->scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
   }
   glBindVertexArray(0);
   glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGlRenderer::terminate() {
  for (const auto& it : shaders) {
		glDeleteProgram(it.second->programHandle);
	}
  for (const auto& it : textures) {
		glDeleteTextures(1, &it.second->id);
  }
}

void OpenGlRenderer::generateTexture(const std::string& name, RawTexture* texture) {
  textures[name] = std::unique_ptr<OpenGLTexture>(
    new OpenGLTexture(texture)
  );
  delete texture;
}

void OpenGlRenderer::generateFontChar(unsigned char c, Math::vec2f size,
 Math::vec2f bearing, unsigned advance, std::vector<unsigned char>* data) {

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
  GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, &(*data)[0]
  );
  // Set texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  font[c] = std::unique_ptr<FontChar>(new FontChar(texture, size, bearing,
   advance));
  delete data;
}

void OpenGlRenderer::test(std::string fontName) {

}

void OpenGlRenderer::addSpriteSurface(float* vertices, const std::string& name) {
  GLuint VAO, VBO, EBO;
	unsigned indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
  };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices,
   GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
   indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
     4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
   (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	spriteSurfaces[name] = VAO;
}



OpenGLTexture::OpenGLTexture(RawTexture* rawTexture):
 width(rawTexture->size.x), height(rawTexture->size.y),
 internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_REPEAT),
 wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMax(GL_LINEAR) {
  glGenTextures(1, &this->id);

  glBindTexture(GL_TEXTURE_2D, this->id);

  glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0,
     this->imageFormat, GL_UNSIGNED_BYTE, &rawTexture->data[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void OpenGLShaderProgram::use() {
	glUseProgram(this->programHandle);
}

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertex_shader_file,
  const std::string& fragment_shader_file) {
	GLuint vertexShader = createShader(GL_VERTEX_SHADER,
   *Utils::PATH_SHADERS_OPENGL + vertex_shader_file);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER,
   *Utils::PATH_SHADERS_OPENGL + fragment_shader_file);

	GLint success;
	GLchar infoLog[512];
	this->programHandle = glCreateProgram();

	glAttachShader(this->programHandle, vertexShader);
	glAttachShader(this->programHandle, fragmentShader);

	glLinkProgram(this->programHandle);
	glGetProgramiv(this->programHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->programHandle, 512, NULL, infoLog);
		Logger::e(std::string("Shader program linking failed. ") + std::string(infoLog));
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint OpenGLShaderProgram::createShader(GLenum eShaderType,
   const std::string& file_path) {
	std::unique_ptr<std::string> shaderStr =
   std::unique_ptr<std::string>(readFile(file_path));
	const GLchar* shaderCode = shaderStr->c_str();
	GLint success;
	GLchar infoLog[512];
	GLuint shader = glCreateShader(eShaderType);

	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	// Print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		Logger::e(std::string("Shader compilation failed") + std::string(infoLog));
	}
	return shader;
}


void OpenGLShaderProgram::addUniformFloat(const GLchar *name, float value) {
    glUniform1f(glGetUniformLocation(this->programHandle, name), value);
}
void OpenGLShaderProgram::addUniformInt(const GLchar *name, int value) {
    glUniform1i(glGetUniformLocation(this->programHandle, name), value);
}

void OpenGLShaderProgram::addUniformVec2f(const GLchar *name, const Math::vec2f &value) {
    glUniform2f(glGetUniformLocation(this->programHandle, name), value.x, value.y);
}

void OpenGLShaderProgram::addUniformVec3f(const GLchar *name, const Math::vec3f &value) {
    glUniform3f(glGetUniformLocation(this->programHandle, name), value.x, value.y, value.z);
}

void OpenGLShaderProgram::addUniformMatrix4(const GLchar *name, const Math::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(this->programHandle, name), 1, GL_FALSE, &matrix.data[0][0]);
}
