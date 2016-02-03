#ifndef AI_H
#define AI_H

#include "GameObject.h"
#include <vector>
#include "SP3DLC.h"
#include "MessageBoard.h"

namespace MyAI
{
	enum FSM_ONE
	{
		DODGE,
		SWAP,
		AGGRESIVE,
		ASSIST,
		STOP1,
		
	};

	enum FSM_TWO
	{
		SWAP2,
		VOLUNTEER,
		HEAL, // Only for AIs that are medics
		STOP2,
		
	};
	class cAI : public GameObject
	{
	public:
		cAI();
		virtual ~cAI();

		void init();
		void update(double dt);

		FSM_ONE getState2();
		FSM_TWO getState();

		void SetAITarget(cAI* target);

		unsigned int GetTeamID();
		void SetTeamID(unsigned int teamID);
		//void RenderState(Mesh* mesh, std::string text, Color color);
		void SetFightPt(Vector3 fightingPoint);
		Vector3 GetFightPt();

		Vector3 getRandPos();
		FSM_ONE FSM1;
		FSM_TWO FSM2;

		MessageBoard mbController;
		std::string Sender;
		std::string Reciever;
	
	private:
		//Waypoints and states

		unsigned int wayPointIndex;
		int randNum;
		bool arrived;
		bool isFighting;
		bool SwapAI;
		bool Volunteer;
		bool missed;
		float offset;
		int startPoint;
		unsigned int ID;
		unsigned int teamID;

		int timer;
		cAI* target;


		float probability;
		Vector3 vel;
		Vector3 nextPoint;
		Vector3 fightingPoint;
		

		//const double rangeofFOV = 100;
		//const float playerRadius = 0.25;
		//const float enemyRadius = 0.1f;
		//const float proximity = 0.4f;
		
	};
}
#endif