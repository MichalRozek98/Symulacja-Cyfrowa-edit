#include <string>
#include <vector>
#include <iostream>
#include "time.h"
#include "simulation_time.h"
#include "channel.h"
#include "csma_network.h"
#include "simulation.h"



int main(int argc, char* argv[])
{
  CsmaNetwork* network = new CsmaNetwork();
  Channel* channel_of_network = new Channel();
  SimulationTime* supervision_of_simulation_time = new SimulationTime(0);// just declare an simulation time but in Simulation.cpp it will be set properly
  Logger* logger = new Logger();

  std::cout << "Select simulation mode: \n" << "1 - continuously \n" << "2 - stepwise \n";
  int mode_type = 0;

  while(mode_type < 1 || mode_type > 2)
  {
    std::cout << "Write number (1-2): ";
    std::cin >> mode_type;
    std::cout << std::endl;
  }

  switch (mode_type)
  {
  case 1:
    network->set_stepwise(false);
    break;
  case 2:
    network->set_stepwise(true);
    break;
  }

  for (int i = 0; i < network->return_kreceiver_transmitter_count(); ++i) // creating transmitters and receivers with correct ids
  {
    Transmitter* transmitter_one = new Transmitter(network->return_vector_of_transmitters().size());
    Receiver* receiver_one = new Receiver(network->return_vector_of_receivers().size());
    network->PushBackToVectorOfTransmitters(transmitter_one);
    network->PushBackToVectorOfReceivers(receiver_one);
  }

  std::cout << "Select log type: \n" << "1 - Only INFO\n" << "2 - Only ERROR\n" << "3 - INFO and ERROR" << std::endl;
  int log_type = 0;

  while (log_type < 1 || log_type > 3)
  {
    std::cout << "Write number (1-3): ";
    std::cin >> log_type;
    std::cout << std::endl;
  }

  switch (log_type)
  {
  case 1:
    logger->SetLogsType(Logger::InformationInConsole::Info);
      break;
  case 2:
    logger->SetLogsType(Logger::InformationInConsole::Error);
      break;
  case 3:
    logger->SetLogsType(Logger::InformationInConsole::InfoAndError);
      break;
  }

  Simulation* Simulation_in_progress = new Simulation(network, channel_of_network, supervision_of_simulation_time, logger);
  Simulation_in_progress->Execute();

  return 0;
}


