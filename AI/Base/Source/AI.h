#ifndef AI_H
#define AI_H

#include "GameObject.h"
#include <vector>
#include "SP3DLC.h"
#include "MessageBoard.h"


	
class cAI : public GameObject
{
	enum FSM_ONE
	{
		STATE_ATTACK,
		STATE_MOVE,
		STATE_DODGE,
		FSM1_NIL,
	};

	enum FSM_TWO
	{
		STATE_HEAL,
		STATE_SWAPOUT,
		STATE_SWAPIN,
		FSM2_NIL,
	};

public:
	cAI();
	virtual ~cAI();

	void init();
	void update(double dt);

	FSM_ONE getState2();
	FSM_TWO getState();


	unsigned int GetTeamID();
	void SetTeamID(unsigned int teamID);
	//void RenderState(Mesh* mesh, std::string text, Color color);
	
	void SetFightPt(Vector3 fightingPoint);
	Vector3 GetFightPt();

	
	FSM_ONE FSM1;
	FSM_TWO FSM2;

	MessageBoard mbController;
	std::string Sender;
	std::string Reciever;
	
	Vector3 startwPoint;
	bool Volunteer;
	int health;

	int id;

private:
	//Waypoints and states

	unsigned int wayPointIndex;
	int randNum;
	
	Vector3 vel;
	Vector3 nextPoint;
	Vector3 fightingPoint;
	// where ur starting pos is

	//const double rangeofFOV = 100;
	//const float playerRadius = 0.25;
	//const float enemyRadius = 0.1f;
	//const float proximity = 0.4f;

};
#endif