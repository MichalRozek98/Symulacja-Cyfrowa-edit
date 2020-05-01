#include "simulation_time.h"
#include <iostream>

SimulationTime::SimulationTime(const size_t time): time_now_(0)
{
  total_time_ = time;
}

SimulationTime::~SimulationTime()
{
  time_now_ = 0;
  total_time_ = 0;
}


void SimulationTime::Delay(size_t time, Logger* logger)
{
  logger->Information("Waiting " + std::to_string(time) + "...");
  size_t time_delay = 0;
  while(time_delay < time)
  {
    ++time_delay;
  }
  time_now_ += time;
}
