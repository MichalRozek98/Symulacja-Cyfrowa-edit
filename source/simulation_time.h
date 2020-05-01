#ifndef SIMULATIONTIME_H_
#define SIMULATIONTIME_H_
#include "logger.h"

class SimulationTime
{
public:
  SimulationTime(size_t time);
  ~SimulationTime();

  void set_time_now(size_t time) { time_now_ = time; };

  size_t return_time_now() { return time_now_; };
  size_t return_total_time() { return total_time_; };

  void Delay(size_t time, Logger* logger);

private:
  size_t time_now_; // variable which is responsible for checking time and compare with the total simulation time
  size_t total_time_; // total simulation time
  
};
#endif //SIMULATIONTIME_H_