
#include "receiver.h"

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

bool Receiver::TerProbabilityOfNotCorrectReceived()
{
  std::vector<int> probability_of_sent = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2};

  int ter_error = rand() % 10;

  if(probability_of_sent[ter_error] == 1)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Receiver::ReceivePacketACK(Packet* receive_packet, Logger* logger, bool colission)
{
  if (!this->TerProbabilityOfNotCorrectReceived() && !colission)
  {
    logger->Information("Packet with id: " + std::to_string(receive_packet->return_packet_id()) + " was correctly received from Transmitter: " + std::to_string(this->ReturnIdReceiver()) + "...");
    receive_packet->set_correct_send(true); // change the flag of properly sending packet
    this->ack_notification_ = true;
  }
  else if(colission)
  {
    logger->Error("Packet with id: " + std::to_string(receive_packet->return_packet_id()) + " wasn't correctly received from Transmitter: " + std::to_string(this->ReturnIdReceiver()) + " because of Colission...");
    this->ack_notification_ = false;
  }
  else
  {
    logger->Error("Packet with id: " + std::to_string(receive_packet->return_packet_id()) + " wasn't correctly received from Transmitter: " + std::to_string(this->ReturnIdReceiver()) + " because of TER...");
    this->ack_notification_ = false;
  }

  return this->ack_notification_;
}
