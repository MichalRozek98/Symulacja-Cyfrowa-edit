#include <string>
#include <vector>
#include <iostream>
#include "time.h"
#include "simulation_time.h"
#include "channel.h"
#include "csma_network.h"
#include "simulation.h"
#include "generator.h"
#include <fstream>



int main(int argc, char* argv[])
{
  CsmaNetwork* network = new CsmaNetwork();
  Channel* channel_of_network = new Channel();
  SimulationTime* supervision_of_simulation_time = new SimulationTime(0);// just declare an simulation time but in Simulation.cpp it will be set properly
  Logger* logger = new Logger("SimulationLogs.txt");

  int seed_ter = 99;
  Generator* generator_pt = new Generator(seed_ter);

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

  int seed = 125;
  Generator* generator = new Generator(seed);
  std::vector<int> seed_vector;

  int number_of_rands = 100000;

  for (int i = 0; i < number_of_rands * 240; ++i)
  {
    auto rand_number = generator->Rnd();

    if (i % 100000 == 0)
    {
      auto seed_to_save = generator->return_kernel();
      seed_vector.push_back(seed_to_save);
    }
  }

  int which_set_of_seed = 0;

  while (which_set_of_seed < 1 || which_set_of_seed > 10)
  {
    std::cout << "Choose your set of seeds (1-10): ";
    std::cin >> which_set_of_seed;
    std::cout << std::endl;
  }

  which_set_of_seed--;

  for (int i = 0; i < 24 * which_set_of_seed; ++i)
  {
    seed_vector.pop_back();
  }

    for (int i = 0; i < network->return_kreceiver_transmitter_count(); ++i) // creating transmitters and receivers with correct ids
    {
      auto seed_to_write_uniform = seed_vector.back();
      seed_vector.pop_back();
      auto seed_to_write_exp = seed_vector.back();
      seed_vector.pop_back();
      auto seed_to_write_zero_one = seed_vector.back();
      seed_vector.pop_back();

      Transmitter* transmitter_one = new Transmitter(network->return_vector_of_transmitters().size(), seed_to_write_uniform, seed_to_write_exp, seed_to_write_zero_one);
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

    Simulation* Simulation_in_progress = new Simulation(network, channel_of_network, supervision_of_simulation_time, logger, generator_pt);
    Simulation_in_progress->Execute();

  return 0;
}


