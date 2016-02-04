#include "MessageBoard.h"


MessageBoard::MessageBoard()
{
	message = "";
	receiverAI = "";
	senderAI = "";
}


MessageBoard::~MessageBoard()
{
}

void MessageBoard::SetMsg(std::string message)
{
	this->message = message;
}

std::string MessageBoard::GetMsg()
{
	return message;
}

void MessageBoard::SetSender(std::string senderAI)
{
	this->senderAI = senderAI;
}

std::string MessageBoard::GetSender()
{
	return senderAI;
}

void MessageBoard::SetReceiver(std::string receiverAI)
{
	this->receiverAI = receiverAI;
}

std::string MessageBoard::GetReceiver()
{
	return receiverAI;
}

//To reset the messageboard if there is message to be sent across the AI
void MessageBoard::ResetMessageBoard()
{
	message	= "Nil";
	receiverAI = "Nil";
	senderAI = "Nil";
}
