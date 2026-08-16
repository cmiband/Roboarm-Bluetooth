#include "LoggingService.h"
#include <fstream>

LoggingService::LoggingService(std::string directory) {
	this->directory = directory;
}

void LoggingService::createLog(std::string message) {
	std::ofstream file(this->directory);
	
	file.close();
}