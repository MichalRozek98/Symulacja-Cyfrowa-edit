#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>
#include "receiver.h"
#include "channel.h"
#include "packet.h"
#include "simulation_time.h"
#include "csma_network.h"
#include "logger.h"
#include "generator.h"

class Simulation
{
public:
  Simulation(CsmaNetwork* network, Channel* channel_of_network, SimulationTime* supervision_of_simulation_time, Logger* logger, Generator* generator);
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
  Generator* generator_;
  bool stop_transmission_when_no_packets_;
  double time_to_update_main_clock_;
  size_t generation_packet_time_max_ = 40;
  size_t transmission_packet_time_max_ = 28;
  size_t retransmission_packet_time_max_ = 27;
  size_t waiting_time_when_channel_is_busy_ = 5;
  size_t waiting_time_when_channel_is_busy_pt_ = 10;
  size_t waiting_random_rctpk_time_max_ = 17;
  size_t ack_notification_send_time_max_ = 9;
  size_t which_transmitter_is_sending_ = -1;
  bool is_retransmission_ = false;
  bool waiting_channel_busy_ = false;
  std::vector<Packet*> packets_received_;
  std::vector<Packet*> packets_not_received_;
  double lambda_ = 0.01;
  size_t initial_phase_time_;
  size_t retransmission_count_ = 0;
  size_t average_delay_packet_ = 0;
  size_t average_waiting_packet_exit_from_bufor_ = 0;
};

#endif // SIMULATION_H_