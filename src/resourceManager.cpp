#include <iostream>
#include <sstream>
#include <fstream>

#include "resourceManager.hpp"
#include "utils.hpp"

using namespace Engine;
using namespace Renderer;
using namespace Utils;

std::map<std::string, Texture2D*> ResourceManager::textures;
std::map<std::string, ShaderProgram*> ResourceManager::shaders;
std::map<GLchar, FontChar*> ResourceManager::font;
Engine::RecordTable* ResourceManager::recordTable = new RecordTable();

FontChar::FontChar(GLuint textureId, Math::vec2<GLint>size, Math::vec2<GLint>bearing, GLuint advance):
	textureId(textureId), size(size), bearing(bearing), advance(advance) {
	
}

FontChar::~FontChar() {
	
}

void ResourceManager::init() {
	ResourceManager::loadShader(std::string("t1.v"), std::string("t1.f"), std::string(""), std::string("texture1_shader"));
	ResourceManager::loadShader(std::string("shader.vert"), std::string("shader.frag"), std::string(""), std::string("projectionShader"));
	ResourceManager::loadShader(std::string("textShader.vert"), std::string("textShader.frag"), std::string(""), std::string("textShader"));
	ResourceManager::loadTexture(std::string("terrain_balk_16"), false, std::string("terrainBalk"));
	ResourceManager::loadTexture(std::string("terrain_ladder_16"), false, std::string("terrainLadder"));
	ResourceManager::loadTexture(std::string("barrel1_front_16"), false, std::string("barrelFront1"));
	ResourceManager::loadTexture(std::string("barrel2_front_16"), false, std::string("barrelFront2"));
	ResourceManager::loadTexture(std::string("barrel1_16"), false, std::string("barrel1"));
	ResourceManager::loadTexture(std::string("barrel2_16"), false, std::string("barrel2"));
	ResourceManager::loadTexture(std::string("barrel3_16"), false, std::string("barrel3"));
	ResourceManager::loadTexture(std::string("barrel4_16"), false, std::string("barrel4"));
	ResourceManager::loadTexture(std::string("runRight1_16"), false, std::string("runRight1"));
	ResourceManager::loadTexture(std::string("runRight2_16"), false, std::string("runRight2"));
	ResourceManager::loadTexture(std::string("runLeft1_16"), false, std::string("runLeft1"));
	ResourceManager::loadTexture(std::string("runLeft2_16"), false, std::string("runLeft2"));
	ResourceManager::loadTexture(std::string("jumpLeft_16"), false, std::string("jumpLeft"));
	ResourceManager::loadTexture(std::string("jumpRight_16"), false, std::string("jumpRight"));
	ResourceManager::loadTexture(std::string("climb1_16"), false, std::string("climb1"));
	ResourceManager::loadTexture(std::string("climb2_16"), false, std::string("climb2"));

	ResourceManager::loadFont("open-sans");
	ResourceManager::recordTable->readTable();
}

void ResourceManager::loadShader(const std::string& vertex_shader_file, 
				const std::string& fragment_shader_file, 
				const std::string& fragment_geometry_file, std::string name) {
    shaders[name] = new ShaderProgram(vertex_shader_file, fragment_shader_file, fragment_geometry_file);
}

ShaderProgram* ResourceManager::getShaderProgram(std::string name){
    return shaders[name];
}

void ResourceManager::loadTexture(const std::string& fileName, GLboolean alpha, std::string name) {
    textures[name] = Texture2D::fromFile(fileName, alpha);
}

Texture2D* ResourceManager::getTexture(std::string name) {
    return textures[name];
}

void ResourceManager::clear() {
    for (auto it : shaders) {
		glDeleteProgram(it.second->programHandle);  
		delete it.second;
	}
    for (auto it : textures) {
		glDeleteTextures(1, &it.second->id);
		delete it.second;
        }
}

int k = 0;

int nextToken(unsigned& start, unsigned& end, std::string& str) {
	k++;
	end = str.find(";", start);

//	std::cout<<"k:"<<k<<",str:"<<str.substr(start, end - start)<<", start: "<<start<<", end: "<<end<<std::endl;

	int value = std::stoi(str.substr(start, end - start));

	start = end + 1;

	return value;

}

void ResourceManager::loadFont(std::string fontName) {
	std::string file_path = *Utils::retreive_binary_path();
	file_path += "fonts/" + fontName;
	std::string result;
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try {
		fileStream.open(file_path.c_str());
		std::getline(fileStream, result);
	}
	catch (std::ifstream::failure e) {
		Logger::e("Failed to load texture: " + fontName);
	}
	
//	Texture2D* texture = new Texture2D(false);
	
	unsigned start = 0;
	unsigned end = 0;
	std::string colon = ";";
	if (!result.empty()) {
		for (unsigned k = 0; k < 256; k++) {
			GLfloat width;
			GLfloat height;
			GLfloat left;
			GLfloat top;
			GLuint advance;
			width = nextToken(start, end, result);
			height = nextToken(start, end, result);
			left = nextToken(start, end, result);
			top = nextToken(start, end, result);
			advance = nextToken(start, end, result);
			
			unsigned dim = (unsigned)((width == height) && (width == 0) ? 0 : ((width == 0 ? 1 : width) * (height == 0 ? 1 : height)));
			unsigned char data[(dim * 3)];
			for (unsigned i = 0; i < dim; i++) {
				
				end = result.find(colon, start);
				unsigned char c = nextToken(start, end, result) * 255;
				data[i] = c;
				data[i + 1] = c;
				data[i + 2] = c;
			}

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				width,
				height,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				data
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			texture->generate(width, height, data);
			font[(GLchar)k] = new FontChar(texture, Math::vec2<GLint>(width, height), Math::vec2<GLint>(left, top), advance);
		}
	}
	

}


