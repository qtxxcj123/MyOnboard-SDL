#ifndef _ACK_TOOL_H
#define _ACK_TOOL_H
#include "myType.h"


class ackTools {
public:
	typedef struct ackResponse {
		DCommand cmd;
		Eresponse ack;
	}ackResponse;
public:
	ackTools();
	~ackTools();

	void setAckCmd(DCommand cmd);

	void setAckRep(Eresponse ack);

	bool verifyAck();

	
private:
	ackResponse* ackRs;
};



#endif
