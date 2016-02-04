#pragma once
#include <string>

class MessageBoard
{
public:
	MessageBoard();
	~MessageBoard();

	std::string message;
	std::string senderAI;
	std::string receiverAI;

	void SetMsg(std::string message);
	std::string GetMsg();
	void SetSender(std::string senderAI);
	std::string GetSender();
	void SetReceiver(std::string receiverAI);
	std::string GetReceiver();

	void ResetMessageBoard();
};

