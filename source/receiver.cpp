
#include "receiver.h"
#include "generator.h"

Receiver::Receiver(unsigned id)
{
  id_receiver_ = id;
  ack_notification_ = false;
}

Receiver::~Receiver()
{
}

void Receiver::SetIdReceiver(unsigned int id)
{
  id_receiver_ = id;
}

int Receiver::ReturnIdReceiver()
{
  return  id_receiver_;
}

bool Receiver::ReturnAckNotification(Logger* logger)
{
  if (ack_notification_)
    logger->Information("(ACK Notification) The packet was received correctly...");
  else
    logger->Error("(ACK Notification) The packet wasn't received correctly...");

  return  ack_notification_;
}

bool Receiver::TerProbabilityOfNotCorrectReceived(Generator* generator, double ter)
{
  if(generator->RndZeroOne(ter) == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Receiver::ReceivePacketACK(Packet* receive_packet, Logger* logger, bool colission, Generator* generator, double ter)
{
  if (!this->TerProbabilityOfNotCorrectReceived(generator, ter) && !colission)
  {
    logger->Information("Packet with id: " + std::to_string(receive_packet->return_packet_id()) + " was correctly received from Transmitter: " + std::to_string(this->ReturnIdReceiver()) + "...");
    receive_packet->set_correct_send(true); // change the flag of properly sending packet
    packets_received_.push_back(receive_packet);
    this->ack_notification_ = true;
  }
  else if((!this->TerProbabilityOfNotCorrectReceived(generator, ter) && colission) || (this->TerProbabilityOfNotCorrectReceived(generator, ter) && colission))
  {
    logger->Error("Packet with id: " + std::to_string(receive_packet->return_packet_id()) + " wasn't correctly received from Transmitter: " + std::to_string(this->ReturnIdReceiver()) + " because of Colission...");
    this->TerProbabilityOfNotCorrectReceived(generator, 1);
    this->ack_notification_ = false;
  }
  else
  {
    logger->Error("Packet with id: " + std::to_string(receive_packet->return_packet_id()) + " wasn't correctly received from Transmitter: " + std::to_string(this->ReturnIdReceiver()) + " because of TER...");
    this->ack_notification_ = false;
  }

  return this->ack_notification_;
}

void Receiver::PushBackPacketsNotReceived(Packet* packet)
{
  packets_not_received_.push_back(packet);
}
