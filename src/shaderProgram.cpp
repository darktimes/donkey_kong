#include <sstream>
#include <fstream>
#include "shaderProgram.hpp"
#include "math.hpp"
#include "utils.hpp"

using namespace Renderer;
using namespace Utils;
using namespace std;



string* ShaderProgram::SHADER_PATH = new string("shaders/");


string ShaderProgram::getFullShaderPath(const string& shader_file) {
	return string(*retreive_binary_path() + *ShaderProgram::SHADER_PATH + shader_file);
}

void ShaderProgram::use() {
	glUseProgram(this->programHandle);
}

ShaderProgram::ShaderProgram(const string& vertex_shader_file, const string& fragment_shader_file, const string& geometry_shader_file)
{
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, getFullShaderPath(vertex_shader_file));
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, getFullShaderPath(fragment_shader_file));
	GLuint geometryShader;
	if (!geometry_shader_file.empty()) {
		geometryShader = createShader(GL_GEOMETRY_SHADER, getFullShaderPath(geometry_shader_file));
	}
	GLint success;
	GLchar infoLog[512];
	this->programHandle = glCreateProgram();
	
	glAttachShader(this->programHandle, vertexShader);
	glAttachShader(this->programHandle, fragmentShader);
	if (!geometry_shader_file.empty()) {
		glAttachShader(this->programHandle, geometryShader);
	}

	glLinkProgram(this->programHandle);
	glGetProgramiv(this->programHandle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->programHandle, 512, NULL, infoLog);
		Logger::e(string("Shader program linking failed. ") + string(infoLog));
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (!geometry_shader_file.empty()) {
		glDeleteShader(geometryShader);	
	}
}

GLuint ShaderProgram::createShader(GLenum eShaderType, const string& file_path)
{
	string shaderStr = readFile(file_path);
	const GLchar* shaderCode = shaderStr.c_str();
	GLint success;
	GLchar infoLog[512];
	GLuint shader = glCreateShader(eShaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	// Print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		Logger::e(string("Shader compilation failed") + string(infoLog));
	}
	return shader;
}

string ShaderProgram::readFile(const string& file_path)
{
	string result;
	ifstream shaderFile;	
	shaderFile.exceptions(ifstream::badbit);
	try
	{
		shaderFile.open(file_path.c_str());
		stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		result = shaderStream.str();
	}
	catch (ifstream::failure e)
	{
		Logger::e("Failed to read shader file");
	}
	return result;
}

void ShaderProgram::addUniformFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->programHandle, name), value);
}
void ShaderProgram::addUniformInt(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->programHandle, name), value);
}
void ShaderProgram::addUniformVec2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->programHandle, name), x, y);
}
void ShaderProgram::addUniformVec2f(const GLchar *name, const Math::vec2<GLfloat> &value, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->programHandle, name), value.x, value.y);
}
void ShaderProgram::addUniformVec3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->programHandle, name), x, y, z);
}

void ShaderProgram::addUniformVec3f(const GLchar *name, const Math::vec3<GLfloat> &value, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->programHandle, name), value.x, value.y, value.z);
}
/*
void ShaderProgram::addUniformVec4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->programHandle, name), x, y, z, w);
}

void ShaderProgram::addUniformVec4f(const GLchar *name, const vec4<GLfloat> &value, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->programHandle, name), value.x, value.y, value.z, value.w);
}
*/
void ShaderProgram::addUniformMatrix4(const GLchar *name, const Math::mat4 &matrix, GLboolean useShader) {
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->programHandle, name), 1, GL_FALSE, &matrix.data[0][0]);
}


















