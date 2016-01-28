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
	void SetFromMessage(std::string From);

	std::string GetToMessage();
	void SetToMessage(std::string From);

	void ResetMessageBoard();
private:

	std::string message;
	std::string From;
	std::string To;
	
};

