#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"

void _check_gl_error(const char *file, int line);

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

/**
*	This namespace contains some utility functions
*/
namespace Utils {
	extern const std::string* const PATH_SHADERS_OPENGL;
	extern const std::string* const PATH_SHADERS_VULKAN;
	extern const std::string* const PATH_TEXTURES;
	extern const std::string* const PATH_FONTS;
	extern double fixedTime;

	const std::string* const retreive_binary_path();

	std::string* readFile(const std::string&);
	int writeFile(const std::string& path, const std::string& content);
	double getTime();

	Rendering::RawTexture* parseTexture(const std::string& name);
	/**
	* Contains some level-dependant functions to log stuff with.
	*/
	namespace Logger {

			enum LoggerLevel {system = 0, error = 1, warning = 2, info = 3,
				 verbose = 4};

			void changeLoggerLevel(const LoggerLevel& level);
			void s(const std::string& msg);
			void e(const std::string& msg);
			void w(const std::string& msg);
			void i(const std::string& msg);
			void v(const std::string& msg);

			extern LoggerLevel loggerLevel;

			void printMessage(const LoggerLevel& logger_level, const std::string& msg);

			std::string labelOfLoggerLevel(const LoggerLevel& logger_level);

	};

}
