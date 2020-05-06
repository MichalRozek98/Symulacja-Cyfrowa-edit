#include "simulation.h"
#include <iostream>
#include <fstream>

Simulation::Simulation(CsmaNetwork* network, Channel* channel_of_network, SimulationTime* supervision_of_simulation_time, Logger* logger)
{
  network_ = network;
  channel_of_network_ = channel_of_network;
  supervision_of_simulation_time_ = supervision_of_simulation_time;
  logger_ = logger;
  stop_transmission_when_no_packets_ = false;
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

      if(network_->return_generation_clock() == 0)
      {
        size_t which_transmitter = rand() % 8;
        network_->return_vector_of_transmitters()[which_transmitter]->GeneratePacket(rand() % 200, logger_);
        network_->set_generation_clock(rand() % generation_packet_time_max_);
        network_->set_event_triggered(false);
      }

      channel_of_network_->CheckForCollision(logger_);

      if (network_->return_transmission_clock() == 0)
      {
        AckNotification();
        network_->set_ack_notification_clock(rand() % ack_notification_send_time_max);
        network_->set_event_triggered(false);
      }

      if (network_->return_retransmission_clock() == 0)
      {
        AckNotification();
        network_->set_ack_notification_clock(rand() % ack_notification_send_time_max);
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
      }
      else
      {
        waiting_channel_busy_ = true;
        network_->set_waiting_if_channel_is_busy(waiting_time_when_channel_is_busy);
      }

      if (!waiting_channel_busy_ && is_any_packet_in_buffers && !is_retransmission_ && !channel_of_network_->return_if_is_channel_busy(logger_) && network_->CheckProbabilityPT(logger_))
      {
          StartTransmission();
          network_->set_transmission_clock(rand() % transmission_packet_time_max_);
          network_->set_event_triggered(false);
      }
      else if (!network_->CheckProbabilityPT(logger_))
      {

      }

      if (network_->return_waiting_random_time_rctpk() == 0 && !waiting_channel_busy_ && is_retransmission_ && !channel_of_network_->return_if_is_channel_busy(logger_) && network_->CheckProbabilityPT(logger_))
      {
          Retransmission();
          network_->set_retransmission_clock(rand() % retransmission_packet_time_max_);
          network_->set_event_triggered(false);
          network_->set_waiting_random_time_rctpk(rand() % waiting_random_rctpk_time_max);
      }
      else if (!network_->CheckProbabilityPT(logger_))
      {

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
}

void Simulation::StartSimulation()
{
 logger_->Information("Starting simulation using the method M1...");
 network_->set_generation_clock(rand() % generation_packet_time_max_);
 supervision_of_simulation_time_ = new SimulationTime(rand()%2000 + 10000);
 logger_->Information("Total simulation time has been set to: " + std::to_string(supervision_of_simulation_time_->return_total_time()) + "...");
}

void Simulation::EndSimulation()
{
  logger_->Information("Simulation has ended...");
}

void Simulation::StartTransmission()
{
  logger_->Information("Starting transmission...");
    for (int i = 0; i < network_->return_kreceiver_transmitter_count(); ++i) // loop to search transmitters and receivers to transmitt packets
    {
      if (network_->return_vector_of_transmitters()[i]->return_packet_vector().size() != 0)
      {
        channel_of_network_->GetPacketAndSend(network_->return_vector_of_transmitters()[i]->return_packet_vector()[0],logger_);
        which_transmitter_is_sending = i;

        break;
      }
    }
    channel_of_network_->set_channel_busy(true);
}

void Simulation::Retransmission()
{
  logger_->Information("Starting retransmission...");
  network_->return_vector_of_transmitters()[which_transmitter_is_sending]->RetransmissionPacket(network_->return_vector_of_transmitters()[which_transmitter_is_sending]->return_packet_vector()[0], logger_); // initialize the packet_to_retransmiss
  channel_of_network_->GetPacketAndSend(network_->return_vector_of_transmitters()[which_transmitter_is_sending]->return_packet_vector()[0], logger_);
  channel_of_network_->set_channel_busy(true);
}

void Simulation::AckNotification()
{
  network_->set_transmission_clock(-1);
  network_->set_retransmission_clock(-1);
  network_->return_vector_of_receivers()[which_transmitter_is_sending]->ReceivePacketACK(network_->return_vector_of_transmitters()[which_transmitter_is_sending]->return_packet_vector()[0], logger_, channel_of_network_->return_colision_status());
}

void Simulation::EndTransmission()
{
  network_->set_transmission_clock(-1);
  network_->set_retransmission_clock(-1);
  network_->set_ack_notification_clock(-1);

  if (network_->return_vector_of_receivers()[which_transmitter_is_sending]->ReturnAckNotification(logger_)) // watch if packet sent correctly
  {
    network_->return_vector_of_receivers()[which_transmitter_is_sending]->PushBackToVectorPacektsReceived(channel_of_network_->ReturnPacketInProgress()[0]); // push to vector of sent packets
    network_->return_vector_of_transmitters()[which_transmitter_is_sending]->return_packet_vector().erase(network_->return_vector_of_transmitters()[which_transmitter_is_sending]->return_packet_vector().begin());

    channel_of_network_->ReturnPacketInProgress().erase(channel_of_network_->ReturnPacketInProgress().begin()); // delete packet from channel to allow to send next packets
    channel_of_network_->set_channel_busy(false);
    is_retransmission_ = false;
    logger_->Information("The transmission has ended..."); // when transmission has ended write this in logs
  }
  else // if ACK is false try to retransmiss packet
  {
    if (channel_of_network_->ReturnPacketInProgress()[0]->return_current_number_of_retransmission() < 15)
    {
      //network_->set_retransmission_clock(rand() % retransmission_packet_time_max_);
      channel_of_network_->ReturnPacketInProgress().erase(channel_of_network_->ReturnPacketInProgress().begin());
      channel_of_network_->set_channel_busy(false);
      is_retransmission_ = true;
    }
    else
    {
      channel_of_network_->ReturnPacketInProgress().erase(channel_of_network_->ReturnPacketInProgress().begin()); // delete packet from channel to allow to send next packets
      channel_of_network_->set_channel_busy(false);
      is_retransmission_ = false;
      logger_->Information("The transmission has ended..."); // when transmission has ended write this in logs
    }
  }
}

void Simulation::UpdateClock()
{
  // update all clocks
  network_->set_transmission_clock(network_->return_transmission_clock() - 1);
  network_->set_generation_clock(network_->return_generation_clock() - 1);
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
  logger_->Information("Actual simulation clock: " + std::to_string(supervision_of_simulation_time_->return_time_now()) + "...");
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
  network_->set_generation_clock(-1);
  network_->set_transmission_clock(-1);
  network_->set_retransmission_clock(-1);
  network_->set_ack_notification_clock(-1);
  network_->set_waiting_if_channel_is_busy(0);
  network_->set_waiting_random_time_rctpk(rand() % waiting_random_rctpk_time_max);
  network_->set_event_triggered(false);
  channel_of_network_->set_status_of_colission(false);
  is_retransmission_ = false;
  waiting_channel_busy_ = false;
  which_transmitter_is_sending = -1;
}
