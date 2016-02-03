#pragma once
#include <string>
class MessageBoard
{
public:
	MessageBoard();
	~MessageBoard();

	std::string Getmessage();
	void SetMessage(std::string message);

	std::string GetFromMessage();
	void SetFromMessage(int id);

	std::string GetToMessage();
	void SetToMessage(int id);

	std::string GetCommand(int index);

	int GetSenderID();
	void SetSenderID(unsigned int senderid);

	void ResetMessageBoard();
private:

	std::string message;
	std::string From;
	std::string To;
	std::string Commands[4];

	unsigned int senderid;
	
};

