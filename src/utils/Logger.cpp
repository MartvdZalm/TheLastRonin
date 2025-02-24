#include "utils/Logger.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string GetCurrentTime()
{
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    std::ostringstream timeStream;
    timeStream << std::setfill('0')
               << std::setw(2) << localTime->tm_hour << ":"
               << std::setw(2) << localTime->tm_min << ":"
               << std::setw(2) << localTime->tm_sec;

    return timeStream.str();
}

void Logger::Info(std::string message)
{
	std::cout << GetCurrentTime() << " [INFO] " << message << std::endl;
}

void Logger::Warning(std::string message)
{
	std::cout << GetCurrentTime() << " [WARNING] " << message << std::endl;
}

void Logger::Error(std::string message)
{
	std::cout << GetCurrentTime() << " [ERROR] " << message << std::endl;
}
