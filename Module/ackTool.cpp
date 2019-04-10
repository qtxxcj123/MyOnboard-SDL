#include "ackTools.h"
ackTools::ackTools() :ackRs(NULL)
{
	ackRs = new ackResponse();
	ackRs->ack = UP_NONE_ACK;
	ackRs->cmd = DOWN_NONE_CMD;
}
ackTools::~ackTools()
{
	delete ackRs;
}

void ackTools::setAckCmd(DCommand cmd)
{
	ackRs->cmd = cmd;
}

void ackTools::setAckRep(Eresponse ack)
{
	ackRs->ack = ack;
}

bool ackTools::verifyAck()
{
	while (ackRs->ack == ackRs->cmd)return true;
	return false;
}

