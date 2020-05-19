#include "csma_network.h"
#include "generator.h"

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

bool CsmaNetwork::CheckProbabilityPT(Logger* logger, bool was_written, Generator* generator)
{
  if(generator->RndZeroOne(pt_probability_) == 0)
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

bool CsmaNetwork::WaitForNextGap(Logger* logger, Generator* generator)
{
  if (generator->RndZeroOne(1 - pt_probability_) == 1)
  {
    logger->Information("Waiting for next gap...");
    return true;
  }
  else
    return false;
}
