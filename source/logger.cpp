#include "logger.h"
#include <string>

Logger::Logger(std::string path)
{
  path_logs_.open(path);
}

Logger::~Logger()
{
  path_logs_.close();
}

void Logger::SetLogsType(InformationInConsole logs_type)
{
  logs_type_ = logs_type;
}

void Logger::Information(std::string info)
{
  if((logs_type_ == InformationInConsole::Info) || (logs_type_ == InformationInConsole::InfoAndError))
  {
    std::cout << "[INFO]: " << info << std::endl;
    path_logs_ << "[INFO]: " << info << std::endl;
  }
}

void Logger::Error(std::string error)
{
  if ((logs_type_ == InformationInConsole::Error) || (logs_type_ == InformationInConsole::InfoAndError))
  {
    std::cout << "[ERROR]: " << error << std::endl;
    path_logs_ << "[ERROR]: " << error << std::endl;
  }
}
