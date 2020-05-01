#ifndef LOGGER_H_
#define LOGGER_H_
#include <fstream>
#include <string>
#include <iostream>

class Logger
{
public:
  enum class InformationInConsole {Info, Error, InfoAndError};

  Logger();
  ~Logger();

  void SetLogsType(InformationInConsole logs_type);

  void Information(std::string info);
  void Error(std::string error);

  std::ofstream path_logs_;
  InformationInConsole logs_type_ = InformationInConsole::Info;

private:

};

#endif // LOGGER_H_
