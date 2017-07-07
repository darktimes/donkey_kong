#include <unistd.h> //for readlink
#include "utils.hpp"

using namespace Utils;


Logger* Logger::instance = nullptr;

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



void Logger::init(const LoggerLevel level) {

	if (!Logger::instance) {
		Logger::instance = new Logger(level);
	} else if (level != Logger::instance->loggerLevel) {
		Logger::instance->loggerLevel = level;
		Logger::instance->printMessage(system, "Logger-Level changed to: " + Logger::labelOfLoggerLevel(level));
	} else {
		Logger::instance->printMessage(system, "Tried to re-init logger with same level");
	}
}

void Logger::terminate() {
	if (!Logger::instance) {
		throw std::runtime_error("Can not terminate not-inited logger.");
	} else {
		delete Logger::instance;
	}
}

void Logger::s(std::string msg) {
	Logger::instance->printMessage(system, msg);
}

void Logger::e(std::string msg) {
	if (Logger::instance->loggerLevel >= error) {
		Logger::instance->printMessage(error, msg);
	} 
}

void Logger::w(std::string msg) {
	if (Logger::instance->loggerLevel >= warning) {
		Logger::instance->printMessage(warning, msg);
	}
}

void Logger::i(std::string msg) {
	if (Logger::instance->loggerLevel >= info) {
		Logger::instance->printMessage(info, msg);
	}
}

void Logger::v(std::string msg) {
	if (Logger::instance->loggerLevel >= verbose) {
		Logger::instance->printMessage(warning, msg);
	}
}


Logger::Logger(const LoggerLevel logger_level)
{
	this->loggerLevel = logger_level;
}

void Logger::printMessage(LoggerLevel logger_level, std::string msg) {
	std::cout<<(Logger::labelOfLoggerLevel(logger_level))<<": "<<msg<<std::endl;
}



std::string Logger::labelOfLoggerLevel(const LoggerLevel logger_level) {
	switch (logger_level) {
		case system : return "SYSTEM";
		case error : return "ERROR";
		case warning: return "WARNING";
		case info: return "INFO";
		case verbose: return "VERBOSE";
		default: throw std::invalid_argument("Logger was init-ed with unknown level-argument");
	}
}

