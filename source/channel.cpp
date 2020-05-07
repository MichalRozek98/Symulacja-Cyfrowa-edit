#include "channel.h"

Channel::Channel()
{
  collision_ = false;
  is_channel_busy_ = false;
}

Channel::~Channel()
{
}

Packet* Channel::GetPacketAndSend(Packet* packet_from_bufor, Logger* logger)
{
  logger->Information("Getting packet with id: " + std::to_string(packet_from_bufor->return_packet_id()) + " from Transmitter: " + std::to_string(packet_from_bufor->return_packet_receiver_transmitter_id()) + " and trying to send...");
  packets_in_progress_.push_back(packet_from_bufor);
  return packet_from_bufor;
}

void Channel::CheckForCollision(Logger* logger)
{
  if (packets_in_progress_.size() > 1) // simple method of checking if was an collision in channel
  {
    logger->Error("A collision was encountered in the channel...");
    collision_ = true;
  }
  else if (packets_in_progress_.size() == 1)
  {
    collision_ = false;
  }
}

bool Channel::return_if_is_channel_busy(Logger* logger, bool was_written)
{
  if (packets_in_progress_.size() == 0) // simple method of checking if was an collision in channel
  {
    if(!was_written)
    logger->Information("Channel is empty...");

    is_channel_busy_ = false;
  }
  else if (packets_in_progress_.size() >= 1)
  {
    if (!was_written)
    logger->Information("Channel is busy...");

    is_channel_busy_ = true;
  }

  return is_channel_busy_;
}

std::vector<Packet*>& Channel::ReturnPacketInProgress()
{
  return packets_in_progress_;
}

