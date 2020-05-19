#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <vector>
#include "packet.h"
#include "logger.h"
#include "channel.h"
#include "generator.h"

class Receiver
{
public:
  Receiver(unsigned id);
  ~Receiver();

  void SetIdReceiver(unsigned int id);
  int ReturnIdReceiver();

  bool ReturnAckNotification(Logger* logger); // method which will return flag with logs
  bool return_ack_flag() { return ack_notification_; }; // method which will return just flag without logs

  bool TerProbabilityOfNotCorrectReceived(Generator* generator, double ter);

  bool ReceivePacketACK(Packet* receive_packet, Logger* logger, bool collision, Generator* generator, double ter); // returns the ACK confirmation of sucesfully received packet

  std::vector<Packet*>& return_packets_received() { return packets_received_; };

  std::vector<Packet*>& return_packets_not_received() { return packets_not_received_; };
  void PushBackPacketsNotReceived(Packet* packet);

private:
  unsigned int id_receiver_; // identicificator number of receiver which will be equal with transmitter's id
  bool ack_notification_; // send ACK notification about sent packet properly
  std::vector<Packet*> packets_received_;
  std::vector<Packet*> packets_not_received_;

};
#endif //REVEIVER_H_


