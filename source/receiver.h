#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <vector>
#include "packet.h"
#include "logger.h"
#include "channel.h"

class Receiver
{
public:
  Receiver(unsigned id);
  ~Receiver();

  void SetIdReceiver(unsigned int id);
  int ReturnIdReceiver();

  bool ReturnAckNotification(Logger* logger); // method which will return flag with logs
  bool return_ack_flag() { return ack_notification_; }; // method which will return just flag without logs

  bool TerProbabilityOfNotCorrectReceived();

  bool ReceivePacketACK(Packet* receive_packet, Logger* logger, bool collision); // returns the ACK confirmation of sucesfully received packet

private:
  unsigned int id_receiver_; // identicificator number of receiver which will be equal with transmitter's id
  bool ack_notification_; // send ACK notification about sent packet properly

};
#endif //REVEIVER_H_


