#ifndef AI_H
#define AI_H

#include "GameObject.h"
#include <vector>
#include "MessageBoard.h"

class cAI : public GameObject
{
	enum FSM_LEADER
	{
		STATE_LEADER_STANBY,    // Set as default in swtich case statement
		STATE_LEADER_FIGHTING,  // Fight other ai
		STATE_LEADER_RETREAT,   // Go back to start point
		STATE_LEADER_SWAPOUT,   // after someone retreats
	};

	enum FSM_SOLDIER
	{
		STATE_SOLDIER_STANDBY,
		STATE_SOLDIER_FIGHTING,
		STATE_SOLDIER_RETREAT,
		STATE_SOLDIER_SWAPOUT,
	};

	enum FSM_MEDIC // Drop this if its too hard just add another soldier.
	{
		STATE_MEDIC_HEAL,  // Increases the health of ai with low hp
		STATE_MEDIC_GOTO,  // go to the ai that needs hp

	};

public:
	cAI();
	virtual ~cAI();

	void init();
	void update(double dt);
	void UpdateLeaderFSM(double dt);
	void UpdateSoldierFSM(double dt);
	void UpdateMedicFSM(double dt);
	
	FSM_LEADER LEADER_FSM;
	FSM_SOLDIER SOLDIER_FSM;
	FSM_MEDIC MEDIC_FSM;


	//Get and set functions
	int getID();
	void setID(int id);

	Vector3 getFinishP();
	void setFinishP(Vector3 fightpoint);

private:
	int id;

	Vector3 fightpoint;


};
#endif