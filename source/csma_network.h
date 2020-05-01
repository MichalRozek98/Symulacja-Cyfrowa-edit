#ifndef CSMANETWORK_H_
#define CSMANETWORK_H_

#include <vector>
#include "receiver.h"
#include "transmitter.h"
#include "packet.h"

class CsmaNetwork
{
public:
  CsmaNetwork();
  ~CsmaNetwork();

  std::vector<Packet*> return_packets_sent() { return packets_sent_; };
  void PushBackToPacketsSent(Packet* packet);

  std::vector<Transmitter*> return_vector_of_transmitters() { return transmitters_; };
  void PushBackToVectorOfTransmitters(Transmitter* transmitter);

  std::vector<Receiver*> return_vector_of_receivers() { return receivers_; };
  void PushBackToVectorOfReceivers(Receiver* receiver);

  void set_event_triggered(bool flag) { event_triggered_ = flag; }; // set the event_triggered_ flag
  bool return_event_triggered() { return event_triggered_; }; // return value of event_triggered_ flag

  int return_kreceiver_transmitter_count() { return kReceiverTransmitterCount_; }; // return value of number k of count Transmitters and Receivers

  void set_generation_clock(size_t clock) { generation_clock_ = clock; };
  void set_transmission_clock(size_t clock) { transmission_clock_ = clock; };
  void set_retransmission_clock(size_t clock) { retransmission_clock_ = clock; };
  void set_waiting_if_channel_is_busy(size_t clock) { waiting_if_channel_is_busy_ = clock; };
  void set_ack_notification_clock(size_t clock) { ack_notification_clock_ = clock; };
  void set_waiting_random_time_rctpk(size_t clock) { waiting_random_time_rctpk_ = clock; };
  void set_stepwise(bool flag) { stepwise_ = flag; };

  size_t return_generation_clock() { return generation_clock_; };
  size_t return_transmission_clock() { return transmission_clock_; };
  size_t return_retransmission_clock() { return retransmission_clock_; };
  size_t return_waiting_if_channel_is_busy() { return waiting_if_channel_is_busy_; };
  size_t return_ack_notification_clock() { return ack_notification_clock_; };
  size_t return_waiting_random_time_rctpk() { return waiting_random_time_rctpk_; };
  bool return_stepwise() { return stepwise_; };

  bool CheckProbabilityPT(Logger* logger);


private:
  static const unsigned int kReceiverTransmitterCount_ = 8; // count of receivers K equal 8 (in my example)
  bool event_triggered_; // flag of event triggered in reviewing activities method
  std::vector<Packet*> packets_sent_; // vector for packets which will be compare with packets received
  std::vector<Transmitter*> transmitters_; // vector of all transmitters
  std::vector<Receiver*> receivers_; // vector of all receivers
  size_t generation_clock_ = -1;
  size_t transmission_clock_ = -1;
  size_t retransmission_clock_ = -1;
  size_t ack_notification_clock_ = -1;
  size_t waiting_if_channel_is_busy_ = 0;
  size_t waiting_random_time_rctpk_ = 0;
  bool stepwise_;

};
#endif //CSMANETWORK_H_;

