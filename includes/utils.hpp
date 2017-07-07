#pragma once

#include <string.h>
#include <iostream>

namespace Utils {

//	std::unique_ptr<std::string> binary_path;
	const std::string* const retreive_binary_path();
//	void retreive_binary_path();


	class Logger {
		public:
			enum LoggerLevel {system = 0, error = 1, warning = 2, info = 3, verbose = 4};
	
			static void init(const LoggerLevel level);
			static void terminate();
			static void s(std::string msg);
			static void e(std::string msg);
			static void w(std::string msg);
			static void i(std::string msg);
			static void v(std::string msg);
	
			static Logger* instance;
	
		private:
			LoggerLevel loggerLevel;

			Logger(const LoggerLevel logger_level);
			void printMessage(LoggerLevel logger_level, std::string msg);

			static std::string labelOfLoggerLevel(const LoggerLevel logger_level);

	};

}



