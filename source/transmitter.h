#ifndef TRANSMITTER_H_
#define TRANSMITTTER_H_

#include <vector>
#include "packet.h"
#include "time.h"
#include "logger.h"
#include "generator.h"

class Transmitter
{
public:
  Transmitter(size_t id, int seed_uniform, int seed_exp, int seed_zero_one);
  ~Transmitter();

  void GeneratePacket(size_t packet_size, Logger* logger, size_t actual_clock);

  Packet* SendPacket(Packet* current_packet_to_send, Logger* logger);
  // get the packet and send it to channel for sending to receiver but first of all push it to bufor


  Packet* RetransmissionPacket(Packet* packet_retransmission, Logger* logger); // try to retransmission packet

  
  size_t return_id() { return id_transmitter_; }; // return id of transmitter
  std::vector<Packet*>& return_packet_vector() { return packets_to_send_; }; // return packets vector

  size_t return_clock() { return clock_; };
  void set_clock(size_t time) { clock_ = time; };

  Generator* return_uniform_generator() { return uniform_generator_; };
  Generator* return_exp_generator() { return exp_generator_; };
  Generator* return_zero_one_generator() { return zero_one_generator_; };

private:
  size_t id_transmitter_; // identicificator number of receiver which will be equal with receiver's id
  std::vector<Packet*> packets_to_send_; // vector of packets
  size_t clock_ = -1;
  Generator* uniform_generator_;
  Generator* exp_generator_;
  Generator* zero_one_generator_;
};

#endif // TRANSMITTER_H_

