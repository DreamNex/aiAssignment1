#include "MessageBoard.h"


MessageBoard::MessageBoard()
{
	message = "";
	receiverAI = "";
	senderAI = "";

	SenderLeader = "Leader ";
	ReceiverLeader = "Leader ";

	SenderMedic = "Medic ";
	ReceiverMedic = "Medic ";

	SenderSoldier = "Soldier ";
	ReceiverSoldier = "Soldier ";
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

//==============================================================================
std::string MessageBoard::GetReceiver()
{
	return receiverAI;
}

std::string MessageBoard::GetLeaderMsg()
{
	return LeaderMSG;
}

std::string MessageBoard::GetMedicMsg()
{
	return MedicMSG;
}

std::string MessageBoard::GetSoldierMsg()
{
	return SoldierMSG;
}
//==================================================================================

void MessageBoard::SetLeaderM(std::string LeaderMSG)
{
	this->LeaderMSG = LeaderMSG;
}
void MessageBoard::SetSoldierM(std::string SoldierMSG)
{
	this->SoldierMSG = SoldierMSG;
}
void MessageBoard::SetMedicM(std::string MedicMSG)
{
	this->MedicMSG = MedicMSG;
}
//To reset the messageboard if there is message to be sent across the AI
void MessageBoard::ResetMessageBoard()
{
	message	= "Nil";
	receiverAI = "Nil";
	senderAI = "Nil";
}
