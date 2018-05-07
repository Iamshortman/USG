#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <stdio.h>
#include <string>

class Logger
{
public:
	static Logger* getInstance();

	void log(const char* format, ...);
	void logDebug(const char* format, ...);
	void logError(const char* format, ...);

	void openLogFile(std::string file_name);
	void closeLogFile();

private:
	//Instance for the Singleton design pattern;
	static Logger* instance;
	Logger();

	FILE* log_file = nullptr;
};


#endif //LOGGER_HPP