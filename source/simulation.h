#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>
#include "receiver.h"
#include "channel.h"
#include "packet.h"
#include "simulation_time.h"
#include "csma_network.h"
#include "logger.h"

class Simulation
{
public:
  Simulation(CsmaNetwork* network, Channel* channel_of_network, SimulationTime* supervision_of_simulation_time, Logger* logger);
  ~Simulation();
  void Execute();
  void StartSimulation();
  void EndSimulation();
  void StartTransmission();
  void Retransmission();
  void AckNotification();
  void EndTransmission();
  void UpdateClock();
  void Initialize();

private:
  CsmaNetwork* network_;
  Channel* channel_of_network_;
  SimulationTime* supervision_of_simulation_time_;
  Logger* logger_;
  bool stop_transmission_when_no_packets_;
  double time_to_update_main_clock_;
  size_t generation_packet_time_max_ = 15;
  size_t transmission_packet_time_max_ = 28;
  size_t retransmission_packet_time_max_ = 27;
  size_t waiting_time_when_channel_is_busy = 5;
  size_t waiting_random_rctpk_time_max = 17;
  size_t ack_notification_send_time_max = 9;
  size_t which_transmitter_is_sending = -1;
  bool is_retransmission_ = false;
  bool waiting_channel_busy_ = false;

};

#endif // SIMULATION_H_