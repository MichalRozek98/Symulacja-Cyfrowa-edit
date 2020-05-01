#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <vector>
#include "packet.h"
#include "logger.h"

class Channel
{
public:
  Channel();
  ~Channel();
  
  Packet* GetPacketAndSend(Packet* packet_from_bufor, Logger* logger);
  // get the packet from bufor (or packet which is sending again) and send to receiver
 
  void CheckForCollision(Logger* logger);
  bool return_if_is_channel_busy(Logger* logger);

  std::vector<Packet*>& ReturnPacketInProgress();

  int return_k_max_retransmission_tries() { return kMaxRetrasmissionTries_; };

  void set_channel_busy(bool status) { is_channel_busy_ = status; };
  void set_status_of_colission(bool status) { collision_ == status; };

  bool return_colision_status() { return collision_; };

private:
  std::vector<Packet*> packets_in_progress_; // using to check if is an collision, we will push in this vector packets and after send we will delete
  static const unsigned int kMaxRetrasmissionTries_ = 15; // max retrasmission tries LR equal 15 (in my example)
  bool is_channel_busy_; // if channel is busy we have to wait
  bool collision_; // checking in channel checking for collision
};
#endif //CHANNEL_H_

