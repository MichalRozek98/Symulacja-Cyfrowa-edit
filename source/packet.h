#ifndef PACKET_H_
#define PACKET_H_
class Packet
{
public:
  Packet();
  ~Packet();

  void set_current_number_of_retransmission(size_t number) { current_number_of_retransmission_ = number; };
  size_t return_current_number_of_retransmission() { return current_number_of_retransmission_; };

  void set_status_of_sending_packet(bool status) { status_of_sending_packet_ = status; };
  bool return_status_of_sending_packet() { return status_of_sending_packet_; };

  void set_packet_size(size_t size) { packet_size_ = size; };
  size_t return_packet_size() { return packet_size_; };

  void set_packet_id(size_t id) { packet_id_ = id; };
  size_t return_packet_id() { return packet_id_; };

  void set_packet_receiver_transmitter_id(size_t id) { packet_receiver_transmitter_id_ = id; };
  size_t return_packet_receiver_transmitter_id() { return packet_receiver_transmitter_id_; };

  void set_correct_send(bool flag) { correct_send_ = flag; };
  bool return_correct_send() { return correct_send_; };
  
private:
  size_t current_number_of_retransmission_; // current number of retransmission of this packet
  bool status_of_sending_packet_; // check if total packet sent
  size_t packet_size_; // packet size
  size_t packet_id_; // packet id
  size_t packet_receiver_transmitter_id_; // need to compare IDs of receiver, transmitter and packet
  bool correct_send_; // watch if the packet was correctly sent 
};
#endif // PACKET_H_


