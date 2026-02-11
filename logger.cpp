// logger.cpp
#include "logger.h"
#include <fstream>
#include <ctime>

void Logger::Init()
{
    std::ofstream("rewind.log", std::ios::app);
}

void Logger::Log(const std::string& msg)
{
    std::ofstream f("rewind.log", std::ios::app);
    f << msg << std::endl;
}
