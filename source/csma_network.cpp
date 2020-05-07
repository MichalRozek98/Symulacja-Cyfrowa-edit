#include "csma_network.h"

CsmaNetwork::CsmaNetwork()
{
  event_triggered_ = false;
}

CsmaNetwork::~CsmaNetwork()
{
}

void CsmaNetwork::PushBackToPacketsSent(Packet* packet)
{
  packets_sent_.push_back(packet);
}

void CsmaNetwork::PushBackToVectorOfTransmitters(Transmitter* transmitter)
{
  transmitters_.push_back(transmitter);
}

void CsmaNetwork::PushBackToVectorOfReceivers(Receiver* receiver)
{
  receivers_.push_back(receiver);
}

bool CsmaNetwork::CheckProbabilityPT(Logger* logger, bool was_written)
{
  srand((int)time(NULL));
  std::vector<size_t> check = {1, 1, 0, 1, 1};
  size_t random = rand() % 5;

  if(check[random] == 0)
  {
    if(!was_written)
    logger->Error("Couldn't send packet because of PT probability...");

    return false;
  }
  else
  {
    if(!was_written)
    logger->Information("Can send packet because of PT probability...");

    return true;
  }
}

bool CsmaNetwork::WaitForNextGap(Logger* logger)
{
  srand((int)time(NULL));
  std::vector<size_t> check = { 1, 1, 0, 1, 1 };
  size_t random = rand() % 5;

  if (check[random] == 0)
  {
    logger->Error("Waiting for next gap...");
    return true;
  }
  else
  {
    return false;
  }
}
