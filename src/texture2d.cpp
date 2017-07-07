#include <iostream>
#include <sstream>
#include <fstream>

#include "texture2d.hpp"
#include "utils.hpp"

using namespace Renderer;
using namespace Utils;

Texture2D::Texture2D(bool alpha)
    : width(0), height(0), internalFormat(alpha ? GL_RGBA : GL_RGB),
      imageFormat(alpha ? GL_RGBA : GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT),
      filterMin(GL_LINEAR), filterMax(GL_LINEAR)
{
    glGenTextures(1, &this->id);
}

void Texture2D::generate(GLuint width, GLuint height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}

Texture2D* Texture2D::fromFile(const std::string& fileName, bool alpha) {
	
	std::string file_path = *retreive_binary_path();
	file_path += "textures/" + fileName;
	std::string result;
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try {
		fileStream.open(file_path.c_str());
		std::getline(fileStream, result);
	}
	catch (std::ifstream::failure e) {
		Logger::e("Failed to load texture: " + fileName);
	}
	
	Texture2D* texture = new Texture2D(alpha);
	
	if (!result.empty()) {
		GLuint width;
		GLuint height;
				
		std::string colon = ";";
		std::string coma = ",";
		int start  = 0;	
		size_t end = result.find(colon, start);
		width = (GLuint)stoi(result.substr(start, end));
		start = end + 1;
		end = result.find(colon, start);
		height = (GLuint)stoi(result.substr(start, end));
		start = end + 1;
		unsigned char data[width * height * 3];
		for (unsigned i = 0; i < width * height * 3; i++) {
			end = result.find(coma, start);
			unsigned char c = (unsigned char)stoi(result.substr(start, end));
			start = end + 1;
			data[i] = c;
			
		}
		texture->generate(width, height, data);
	}

	//
	return texture;
}
