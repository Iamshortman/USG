#include "Logger.hpp"

#include <stdarg.h>

Logger* Logger::instance = nullptr;
Logger* Logger::getInstance()
{
	if (Logger::instance == nullptr)
	{
		Logger::instance = new Logger();
	}

	return Logger::instance;
}

Logger::Logger()
{

}

void Logger::log(const char* format, ...)
{
	va_list args;
	int len;
	char* buffer = nullptr;

	va_start(args, format);
	len = _vscprintf(format, args) + 1;
	buffer = (char*)malloc(len * sizeof(char));
	vsprintf_s(buffer, len, format, args);

	printf("%s", buffer);

	if (this->log_file != nullptr)
	{
		fprintf(this->log_file, "%s", buffer);
	}

	free(buffer);
}

void Logger::logDebug(const char* format, ...)
{
	va_list args;
	int len;
	char* buffer = nullptr;

	va_start(args, format);
	len = _vscprintf(format, args) + 1;
	buffer = (char*)malloc(len * sizeof(char));
	vsprintf_s(buffer, len, format, args);

	printf("Debug: %s", buffer);

	if (this->log_file != nullptr)
	{
		fprintf(this->log_file, "Debug: %s", buffer);
	}

	free(buffer);
}

void Logger::logError(const char* format, ...)
{
	va_list args;
	int len;
	char* buffer = nullptr;

	va_start(args, format);
	len = _vscprintf(format, args) + 1;
	buffer = (char*)malloc(len * sizeof(char));
	vsprintf_s(buffer, len, format, args);

	printf("Error: %s", buffer);

	if (this->log_file != nullptr)
	{
		fprintf(this->log_file, "Error: %s", buffer);
	}

	free(buffer);
}

void Logger::openLogFile(std::string file_name)
{
	this->closeLogFile();
	fopen_s(&this->log_file, file_name.c_str(), "a");
}

void Logger::closeLogFile()
{
	if (this->log_file != nullptr)
	{
		fclose(this->log_file);
		this->log_file = nullptr;
	}
}
