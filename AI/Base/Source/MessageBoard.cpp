#include "MessageBoard.h"


MessageBoard::MessageBoard()
{
	Commands[0] = "Swap";
	Commands[1] = "Assit Me!";
	Commands[2] = "Medic!";
}


MessageBoard::~MessageBoard()
{
}

int MessageBoard::GetSenderID()
{
	return senderid;
}

void MessageBoard::SetSenderID(unsigned int senderid)
{
	this->senderid = senderid;
}

std::string MessageBoard::GetCommand(int index)
{
	return Commands[index];
}

//Get the message that is meant to sent from AI to AI
std::string MessageBoard::Getmessage()
{
	return message;
}

//Set the message that is meant to send from ai to ai
void MessageBoard::SetMessage(std::string message)
{
	this->message = message;
}

//Get the string that says who sent the message
std::string MessageBoard::GetFromMessage()
{
	return From;
}

//Set the string that says who sent the message
void MessageBoard::SetFromMessage(std::string From)
{
	this->From = From;
}

//Get the string that says who recieve the message
std::string MessageBoard::GetToMessage()
{
	return To;
}

//Set the string that says who recieve the message
void MessageBoard::SetToMessage(std::string To)
{
	this->To = To;
}

//To reset the messageboard if there is message to be sent across the AI
void MessageBoard::ResetMessageBoard()
{
	message	  =   "Nil";
	To        =	  "Nil";
	From      =   "Nil";
}
