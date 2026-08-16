#pragma once
#include <string>

class LoggingService
{
private:
	std::string directory;
public:
	LoggingService(std::string directory);
	void createLog(std::string message);
};

