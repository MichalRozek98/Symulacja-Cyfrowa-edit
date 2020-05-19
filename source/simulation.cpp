#include "simulation.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include "generator.h"

Simulation::Simulation(CsmaNetwork* network, Channel* channel_of_network, SimulationTime* supervision_of_simulation_time, Logger* logger, Generator* generator)
{
  network_ = network;
  channel_of_network_ = channel_of_network;
  supervision_of_simulation_time_ = supervision_of_simulation_time;
  logger_ = logger;
  stop_transmission_when_no_packets_ = false;
  generator_ = generator;
}

Simulation::~Simulation()
{
  network_ = nullptr;
  channel_of_network_ = nullptr;
  supervision_of_simulation_time_ = nullptr;
  logger_ = nullptr;
}

void Simulation::Execute()
{
  srand((int)time(NULL));

  supervision_of_simulation_time_->set_time_now(0);
  Initialize();
  StartSimulation();
  
  //starting simulation
  while(supervision_of_simulation_time_->return_time_now() < supervision_of_simulation_time_->return_total_time()) // main loop of watching time
  {  
    network_->set_event_triggered(false);

    while (!network_->return_event_triggered()) // while loop for doing transmission
    {
      network_->set_event_triggered(true);

      channel_of_network_->CheckForCollision(logger_);

      for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i)
      {
        if (network_->return_vector_of_transmitters()[i]->return_clock() == 0)
        {
          network_->return_vector_of_transmitters()[i]->GeneratePacket(generator_->RandMinMax(0,200), logger_, supervision_of_simulation_time_->return_time_now());
          network_->return_vector_of_transmitters()[i]->set_clock(network_->return_vector_of_transmitters()[i]->return_exp_generator()->RndExp(lambda_));
          network_->set_event_triggered(false);
          break;
        }
      }

      if (network_->return_transmission_clock() == 0)
      {
        AckNotification();
        network_->set_ack_notification_clock(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_uniform_generator()->RandMinMax(0, ack_notification_send_time_max_));
        network_->set_event_triggered(false);
      }

      if (network_->return_retransmission_clock() == 0)
      {
        AckNotification();
        network_->set_ack_notification_clock(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_uniform_generator()->RandMinMax(0, ack_notification_send_time_max_));
        network_->set_event_triggered(false);
      }

      if (network_->return_ack_notification_clock() == 0)
      {
        EndTransmission();
        network_->set_event_triggered(false);
      }

      // checking all buffers in all transmitters if is any packet to send
      bool is_any_packet_in_buffers = false;

      for(int i=0; i<network_->return_kreceiver_transmitter_count(); ++i)
      {
        if (network_->return_vector_of_transmitters()[i]->return_packet_vector().size() != 0)
        {
          is_any_packet_in_buffers = true;
          break;
        }
        else
        {
          is_any_packet_in_buffers = false;
        }
      }

      if (network_->return_waiting_if_channel_is_busy() == 0)
      {
        waiting_channel_busy_ = false;
        network_->set_waiting_if_channel_is_busy(waiting_time_when_channel_is_busy_);
      }
      else
      {
        waiting_channel_busy_ = true;
      }

      if (!waiting_channel_busy_ && is_any_packet_in_buffers && !is_retransmission_ && !channel_of_network_->return_if_is_channel_busy(logger_, false) && network_->CheckProbabilityPT(logger_, false, generator_) )
      {
         StartTransmission();

         if(supervision_of_simulation_time_->return_time_now() >= initial_phase_time_)
            average_waiting_packet_exit_from_bufor_ += supervision_of_simulation_time_->return_time_now() - network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector()[0]->return_time_existing();

         network_->set_transmission_clock(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_uniform_generator()->RandMinMax(0, transmission_packet_time_max_));
         network_->set_event_triggered(false);
      }
      else if (!waiting_channel_busy_ && is_any_packet_in_buffers && !is_retransmission_ && !channel_of_network_->return_if_is_channel_busy(logger_, true) && !network_->CheckProbabilityPT(logger_, true, generator_) )
      {
        if (network_->WaitForNextGap(logger_, generator_))
        {
          network_->set_waiting_if_channel_is_busy(waiting_time_when_channel_is_busy_pt_);
          network_->set_event_triggered(false);
        }  
      }

      if (network_->return_waiting_random_time_rctpk() == 0 && !waiting_channel_busy_ && is_retransmission_ && !channel_of_network_->return_if_is_channel_busy(logger_, false) && network_->CheckProbabilityPT(logger_, false, generator_) )
      {
         Retransmission();
         network_->set_retransmission_clock(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_uniform_generator()->RandMinMax(0, retransmission_packet_time_max_));
         network_->set_event_triggered(false);
         network_->set_waiting_random_time_rctpk(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_uniform_generator()->RandMinMax(0, waiting_random_rctpk_time_max_) * network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_uniform_generator()->RandMinMax(0, network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector()[0]->return_current_number_of_retransmission())); // CTPk * R
      }
      else if (network_->return_waiting_random_time_rctpk() == 0 && !waiting_channel_busy_ && is_retransmission_ && !channel_of_network_->return_if_is_channel_busy(logger_, true) && !network_->CheckProbabilityPT(logger_, true, generator_) )
      {
        if (network_->WaitForNextGap(logger_, generator_))
        {
          network_->set_waiting_if_channel_is_busy(waiting_time_when_channel_is_busy_pt_);
          network_->set_event_triggered(false);
        }
      }
    }

    UpdateClock();

    if(network_->return_stepwise())
    {
      logger_->Information("Press ENTER to continue...");
      std::getchar();
    }
  }

  EndSimulation();

  SaveStatistics();
}

void Simulation::StartSimulation()
{
  int choose_settings = 0;
  std::cout << "Choose settings" << std::endl;
  std::cout << "1 - Default values of input parameters" << std::endl;
  std::cout << "2 - Declare the input parameters" << std::endl;
  
  while (choose_settings < 1 || choose_settings > 2)
  {
    std::cin >> choose_settings;
  }

  if (choose_settings == 2)
  {
    lambda_ = -1;
    ter_probability_ = -1;

    std::cout << "Please enter the lambda value (0-0.1): ";
    while (lambda_ < 0 || lambda_ > 0.1)
    {
      std::cin >> lambda_;
    }
    std::cout << std::endl;
    std::cout << "Please enter the TER value (0-1): ";
    while (ter_probability_ < 0 || ter_probability_ > 1)
    {
      std::cin >> ter_probability_;
    }
    std::cout << std::endl;
  }

  logger_->Information("Set input parameters: ");
  logger_->Information("Lambda: " + std::to_string(lambda_));
  logger_->Information("TER: " + std::to_string(ter_probability_));

 for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i)
 {
   network_->return_vector_of_transmitters()[i]->set_clock(network_->return_vector_of_transmitters()[i]->return_uniform_generator()->RndExp(lambda_));
 }

 size_t simulation_time;
 std::cout << std::endl;
 std::cout << "Write Simulation time [ms]: ";
 std::cin >> simulation_time;
 supervision_of_simulation_time_ = new SimulationTime(simulation_time*10);
 logger_->Information("Total simulation time has been set to: " + std::to_string((double)((double)supervision_of_simulation_time_->return_total_time()/10)) + " ms...");
 std::cout << std::endl;
 std::cout << "Write the time of the initial phase [ms]: ";
 std::cin >> initial_phase_time_;
 initial_phase_time_ *= 10;

 logger_->Information("Starting simulation using the method M1...");
}

void Simulation::EndSimulation()
{
  logger_->Information("Simulation has ended...");
}

void Simulation::StartTransmission()
{
  logger_->Information("Starting transmission...");

      int i = generator_->RandMinMax(0, network_->return_kreceiver_transmitter_count());
      bool watch = false;
      
      do
      {
        while (which_transmitter_is_sending_ == i)
        {
          i = generator_->RandMinMax(0, network_->return_kreceiver_transmitter_count());
        }

        if (network_->return_vector_of_transmitters()[i]->return_packet_vector().size() != 0)
        {
          channel_of_network_->GetPacketAndSend(network_->return_vector_of_transmitters()[i]->return_packet_vector()[0], logger_);
          which_transmitter_is_sending_ = i;

          if (generator_->RndZeroOne(0.05) == 0)
            watch = true;
          else
            watch = false;
        }
        else
        {
          i = generator_->RandMinMax(0, network_->return_kreceiver_transmitter_count());
        }   
      } while (!watch);
      

      /*for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i)
      {
        if (network_->return_vector_of_transmitters()[i]->return_packet_vector().size() != 0)
        {
          channel_of_network_->GetPacketAndSend(network_->return_vector_of_transmitters()[i]->return_packet_vector()[0], logger_);
          which_transmitter_is_sending_ = i;

          if(generator_->RndZeroOne(0.05) == 0)
            break;
        }
      }*/

    channel_of_network_->set_channel_busy(true);
}

void Simulation::Retransmission()
{
  logger_->Information("Starting retransmission...");
  network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->RetransmissionPacket(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector()[0], logger_); // initialize the packet_to_retransmiss
  channel_of_network_->GetPacketAndSend(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector()[0], logger_);
  channel_of_network_->set_channel_busy(true);
}

void Simulation::AckNotification()
{
  network_->set_transmission_clock(-1);
  network_->set_retransmission_clock(-1);
  network_->return_vector_of_receivers()[which_transmitter_is_sending_]->ReceivePacketACK(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector()[0], logger_, channel_of_network_->return_colision_status(), generator_, ter_probability_);
}

void Simulation::EndTransmission()
{
  network_->set_transmission_clock(-1);
  network_->set_retransmission_clock(-1);
  network_->set_ack_notification_clock(-1);

  if (network_->return_vector_of_receivers()[which_transmitter_is_sending_]->ReturnAckNotification(logger_)) // watch if packet sent correctly
  {
    if (supervision_of_simulation_time_->return_time_now() >= initial_phase_time_)
    {
      packets_received_.push_back(channel_of_network_->ReturnPacketInProgress()[0]); // push to vector of sent packets
      retransmission_count_ += channel_of_network_->ReturnPacketInProgress()[0]->return_current_number_of_retransmission(); // update retransmission count
      average_delay_packet_ += supervision_of_simulation_time_->return_time_now() - channel_of_network_->ReturnPacketInProgress()[0]->return_time_existing(); // push time of existing packet after generate to correctly received
    }

    network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector().erase(network_->return_vector_of_transmitters()[which_transmitter_is_sending_]->return_packet_vector().begin());
    channel_of_network_->ReturnPacketInProgress().erase(channel_of_network_->ReturnPacketInProgress().begin()); // delete packet from channel to allow to send next packets
    channel_of_network_->set_channel_busy(false);
    is_retransmission_ = false;
    logger_->Information("The transmission has ended..."); // when transmission has ended write this in logs
  }
  else // if ACK is false try to retransmiss packet
  {
    if (channel_of_network_->ReturnPacketInProgress()[0]->return_current_number_of_retransmission() < 15)
    {
      channel_of_network_->set_channel_busy(false);
      is_retransmission_ = true;
    }
    else
    {
      if (supervision_of_simulation_time_->return_time_now() >= initial_phase_time_)
      {
        network_->return_vector_of_receivers()[which_transmitter_is_sending_]->PushBackPacketsNotReceived(channel_of_network_->ReturnPacketInProgress()[0]);
        packets_not_received_.push_back(channel_of_network_->ReturnPacketInProgress()[0]);
      }

      channel_of_network_->set_channel_busy(false);
      is_retransmission_ = false;
      logger_->Information("The transmission has ended..."); // when transmission has ended write this in logs
    }

    int collision_packets = channel_of_network_->ReturnPacketInProgress().size();
    for (int i = 0; i < collision_packets; ++i)
    {
      channel_of_network_->ReturnPacketInProgress().erase(channel_of_network_->ReturnPacketInProgress().begin());
    }
  }
}

void Simulation::UpdateClock()
{
  // update all clocks
  network_->set_transmission_clock(network_->return_transmission_clock() - 1);

  for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i)
  {
    network_->return_vector_of_transmitters()[i]->set_clock(network_->return_vector_of_transmitters()[i]->return_clock() - 1);
  }

  network_->set_retransmission_clock(network_->return_retransmission_clock() - 1);
  network_->set_ack_notification_clock(network_->return_ack_notification_clock() - 1);

  if (network_->return_waiting_if_channel_is_busy() > 0)
  {
    network_->set_waiting_if_channel_is_busy(network_->return_waiting_if_channel_is_busy() - 1);
  }

  if (network_->return_waiting_random_time_rctpk() > 0 && is_retransmission_)
  {
    network_->set_waiting_random_time_rctpk(network_->return_waiting_random_time_rctpk() - 1);
  }

  supervision_of_simulation_time_->set_time_now(supervision_of_simulation_time_->return_time_now() + 1);
  logger_->Information("Actual simulation clock: " + std::to_string((double)((double)supervision_of_simulation_time_->return_time_now()/10)) + " ms...");
}

void Simulation::Initialize()
{
  for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i) // clearing all bufors of Transmitters
  {
    for(int j=0; j < network_->return_vector_of_transmitters()[i]->return_packet_vector().size(); ++j)
    {
      network_->return_vector_of_transmitters()[i]->return_packet_vector().pop_back();
    }
  }

  channel_of_network_->set_channel_busy(false);

  for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i)
  {
    network_->return_vector_of_transmitters()[i]->set_clock(-1);
  }

  network_->set_transmission_clock(-1);
  network_->set_retransmission_clock(-1);
  network_->set_ack_notification_clock(-1);
  network_->set_waiting_if_channel_is_busy(0);
  network_->set_waiting_random_time_rctpk(rand() % waiting_random_rctpk_time_max_);
  network_->set_event_triggered(false);
  channel_of_network_->set_status_of_colission(false);
  is_retransmission_ = false;
  waiting_channel_busy_ = false;
  which_transmitter_is_sending_ = -1;
  retransmission_count_ = 0;
  average_delay_packet_ = 0;
  average_waiting_packet_exit_from_bufor_ = 0;
}

void Simulation::SaveStatistics()
{
  logger_ = new Logger("Statistics.txt");

  std::cout << "\n";
  std::vector<double> ber_vector;
  double ber_rate = 0;
  double ber_rate_average = 0;
  double ber_rate_max = 0;

  for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i)
  {
    logger_->Information("Statistic for TX & RX number " + std::to_string(i));
    logger_->Information("Packets received properly: " + std::to_string(network_->return_vector_of_receivers()[i]->return_packets_received().size()));
    logger_->Information("Packets received incorrectly: " + std::to_string(network_->return_vector_of_receivers()[i]->return_packets_not_received().size()));
    if (network_->return_vector_of_receivers()[i]->return_packets_received().size() != 0)
    {
      ber_rate = (double)network_->return_vector_of_receivers()[i]->return_packets_not_received().size() / (double)network_->return_vector_of_receivers()[i]->return_packets_received().size();
      ber_vector.push_back(ber_rate);
    }
    logger_->Information("BER: " + std::to_string(ber_rate));
    std::cout << "\n";
  }

  for (int i = 0; i < ber_vector.size(); ++i)
  {
    ber_rate_average += ber_vector[i];

    if (ber_vector[i] > ber_rate_max)
    {
      ber_rate_max = ber_vector[i];
    }
  }


  ber_rate_average /= ber_vector.size();

  logger_->Information("GENERAL STATISTICS");
  std::cout << "\n";

  logger_->Information("Packets received properly: " + std::to_string(packets_received_.size()));
  logger_->Information("Packets received incorrectly: " + std::to_string(packets_not_received_.size()));
  logger_->Information("BER: " + std::to_string(ber_rate_average));
  logger_->Information("Max BER: " + std::to_string(ber_rate_max));
  logger_->Information("Average retransmission count: " + std::to_string((double)retransmission_count_ / (double)packets_received_.size()));
  logger_->Information("Average packet delay: " + std::to_string(((double)average_delay_packet_ / (double)packets_received_.size()) / 1000) + " ms");
  logger_->Information("Average packet waiting exit from bufor: " + std::to_string(((double)average_waiting_packet_exit_from_bufor_ / (double)packets_received_.size()) / 1000) + " ms");
  logger_->Information("Bit rate: " + std::to_string(((double)packets_received_.size() / (double)supervision_of_simulation_time_->return_total_time()) * 1000) + " b/s");
}


