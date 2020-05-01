#include "packet.h"
#include <cstdlib>
#include "time.h"

Packet::Packet()
{
  current_number_of_retransmission_ = 0;
  status_of_sending_packet_ = false;
}

Packet::~Packet()
{
}

