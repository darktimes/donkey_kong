#include <unistd.h> //for readlink
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include "game.hpp"
#include "utils.hpp"
#include "math.hpp"

using namespace Utils;
using namespace Math;

const std::string* const Utils::PATH_SHADERS_OPENGL = new std::string("shaders/opengl/");
const std::string* const Utils::PATH_SHADERS_VULKAN = new std::string("shaders/vulkan/");
const std::string* const Utils::PATH_TEXTURES = new std::string("textures/");
const std::string* const Utils::PATH_FONTS = new std::string("fonts/");
double Utils::fixedTime = 0;

/**
* returns current working directory as string
*/
const std::string* const Utils::retreive_binary_path() {
		std::string fullFileName = "";
		std::string path = "";
		pid_t pid = getpid();
		char buf[20] = {0};
		sprintf(buf,"%d",pid);
		std::string _link = "/proc/";
		_link.append( buf );
		_link.append( "/exe");
		char proc[512];
		int ch = readlink(_link.c_str(),proc,512);
		if (ch != -1) {
			proc[ch] = 0;
			path = proc;
			std::string::size_type t = path.find_last_of("/");
			path = path.substr(0,t);
		}

		return new std::string(path + std::string("/"));
}

/**
* Time function wrapper.
*/
double Utils::getTime() {
  if (Game::getInstance().gameMode == Game::DEFAULT) {
    return glfwGetTime();
  } else {
    return fixedTime;
  }
}

/**
* Reads file content as a string.
* Returns the pointer to the result string.
*/
std::string* Utils::readFile(const std::string& targetPath) {
	std::string path = *Utils::retreive_binary_path() + targetPath;
	std::string *result = new std::string();
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try {
		fileStream.open(path.c_str());
		std::stringstream fileStringStream;
		fileStringStream<<fileStream.rdbuf();
		//std::getline(fileStream, *result);
		fileStream.close();
		*result = fileStringStream.str();
	}
	catch (const std::ifstream::failure& e) {
		Logger::e("Failed to read a file: " + targetPath);
	}
	return result;
}

/**
* Writes given string to the file with specified path. If file was present, it
* gets removed. Returns -1, if something wrong happened, 0 - otherwise.
*/
int Utils::writeFile(const std::string& path, const std::string& content) {
	remove(path.c_str());
	std::ofstream outputStream;
	try {
		outputStream.open(path.c_str());
		outputStream<<content;
		outputStream.close();
	} catch(...) {
		return -1;
	}
	return 0;
}

/**
* Reads the file with given name in the texture folder, reads it, and
* returns the char array in RGB-format. This function works with files,
* that were created with texture-converter utility and expects certain
* file format.
*/
Rendering::RawTexture* Utils::parseTexture(const std::string& fileName) {
	std::string fileString = *(readFile(*PATH_TEXTURES + fileName));

	std::string colon = ";";
	std::string coma = ",";
	int start  = 0;
	size_t end = fileString.find(colon, start);
	GLuint width = (GLuint)stoi(fileString.substr(start, end));
	start = end + 1;
	end = fileString.find(colon, start);
	GLuint height = (GLuint)stoi(fileString.substr(start, end));
	start = end + 1;

	std::vector<unsigned char> *data = new std::vector<unsigned char>();
	for (unsigned i = 0; i < width * height * 3; i++) {
		end = fileString.find(coma, start);
		unsigned char c = (unsigned char)stoi(fileString.substr(start, end));
		start = end + 1;
		data->push_back(c);
	}

	return new Rendering::RawTexture(vec2ui(width, height), data);
}

Logger::LoggerLevel Logger::loggerLevel = verbose;

void Logger::changeLoggerLevel(const LoggerLevel& level) {
	loggerLevel = level;
	if (level != loggerLevel) {
		loggerLevel = level;
		Logger::printMessage(system, "Logger-Level changed to: " + Logger::labelOfLoggerLevel(level));
	} else {
		Logger::printMessage(system, "Tried to re-init logger with same level");
	}
}


void Logger::s(const std::string& msg) {
	Logger::printMessage(system, msg);
}

void Logger::e(const std::string& msg) {
	if (Logger::loggerLevel >= error) {
		Logger::printMessage(error, msg);
	}
}

void Logger::w(const std::string& msg) {
	if (Logger::loggerLevel >= warning) {
		Logger::printMessage(warning, msg);
	}
}

void Logger::i(const std::string& msg) {
	if (Logger::loggerLevel >= info) {
		Logger::printMessage(info, msg);
	}
}

void Logger::v(const std::string& msg) {
	if (Logger::loggerLevel >= verbose) {
		Logger::printMessage(verbose, msg);
	}
}

void Logger::printMessage(const LoggerLevel& logger_level,const std::string& msg) {
	std::cout<<(Logger::labelOfLoggerLevel(logger_level))<<": "<<msg<<std::endl;
}

std::string Logger::labelOfLoggerLevel(const LoggerLevel& logger_level) {
	switch (logger_level) {
		case system : return "SYSTEM";
		case error : return "ERROR";
		case warning: return "WARNING";
		case info: return "INFO";
		case verbose: return "VERBOSE";
		default: throw std::invalid_argument("Logger was init-ed with unknown level-argument");
	}
}

void _check_gl_error(const char *file, int line) {
	GLenum err(glGetError());
	while(err != GL_NO_ERROR) {
    std::string error;
    switch(err) {
            case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
            case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
		Logger::e("GL_" + error + " - " + std::string(file) + std::to_string(line));
    //cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<endl;
    err = glGetError();
	}
}
