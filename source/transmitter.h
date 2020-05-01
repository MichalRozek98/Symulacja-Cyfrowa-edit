#ifndef TRANSMITTER_H_
#define TRANSMITTTER_H_

#include <vector>
#include "packet.h"
#include "time.h"
#include "logger.h"

class Transmitter
{
public:
  Transmitter(size_t id);
  ~Transmitter();

  void GeneratePacket(size_t packet_size, Logger* logger);

  Packet* SendPacket(Packet* current_packet_to_send, Logger* logger);
  // get the packet and send it to channel for sending to receiver but first of all push it to bufor


  Packet* RetransmissionPacket(Packet* packet_retransmission, Logger* logger); // try to retransmission packet

  
  size_t return_id() { return id_transmitter_; }; // return id of transmitter
  std::vector<Packet*>& return_packet_vector() { return packets_to_send_; }; // return packets vector

private:
  size_t id_transmitter_; // identicificator number of receiver which will be equal with receiver's id
  std::vector<Packet*> packets_to_send_; // vector of packets

};

#endif // TRANSMITTER_H_

