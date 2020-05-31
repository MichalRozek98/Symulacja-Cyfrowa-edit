#include "transmitter.h"
#include <iostream>
#include "generator.h"

Transmitter::Transmitter(size_t id, int seed_uniform, int seed_exp, int seed_zero_one)
{
  id_transmitter_ = id;
  uniform_generator_ = new Generator(seed_uniform);
  exp_generator_ = new Generator(seed_exp);
  zero_one_generator_ = new Generator(seed_zero_one);
}

Transmitter::~Transmitter()
{
}

void Transmitter::GeneratePacket(size_t packet_size, Logger* logger, size_t actual_clock)
{
  Packet* packet = new Packet();
  packet->set_time_existing(actual_clock);
  packet->set_packet_size(packet_size); // initialization of packet size
  //packet->SetTimeCgpk(rand() % 1001); // generate a dealy of packet creating from 0 to 1000 ms
  packet->set_packet_id(packets_to_send_.size());
  packet->set_packet_receiver_transmitter_id(this->id_transmitter_);
  logger->Information("Generating a packet by Transmitter: " + std::to_string(this->id_transmitter_) + "...");
  this->packets_to_send_.push_back(packet);
}

Packet* Transmitter::SendPacket(Packet* current_packet_to_send, Logger* logger)
{
  logger->Information("Sending packet from Transmitter: " + std::to_string(this->id_transmitter_) + " to Receiver: " + std::to_string(this->id_transmitter_));
  return current_packet_to_send;
}

Packet* Transmitter::RetransmissionPacket(Packet* packet_retransmission, Logger* logger)
{
  if(packet_retransmission->return_current_number_of_retransmission() < 15)
  {
    logger->Information("Trying to retransmit packet with id: " + std::to_string(packet_retransmission->return_packet_id()) + " from Transmitter: " + std::to_string(this->id_transmitter_) + "...");
    packet_retransmission->set_current_number_of_retransmission(packet_retransmission->return_current_number_of_retransmission() + 1); // increment of retransmission packet
  }

  return packet_retransmission;
}

