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

	std::string LeaderMSG;
	std::string SenderLeader;
	std::string ReceiverLeader;

	std::string MedicMSG;
	std::string SenderMedic;
	std::string ReceiverMedic;

	std::string SoldierMSG;
	std::string SenderSoldier;
	std::string ReceiverSoldier;

	void SetMsg(std::string message);
	std::string GetMsg();
	void SetSender(std::string senderAI);
	std::string GetSender();
	void SetReceiver(std::string receiverAI);
	std::string GetReceiver();

	std::string GetLeaderMsg();
	std::string GetMedicMsg();
	std::string GetSoldierMsg();

	void SetLeaderM(std::string LeaderMSG);
	void SetSoldierM(std::string SoldierMSG);
	void SetMedicM(std::string MedicMSG);

	void ResetMessageBoard();
};

