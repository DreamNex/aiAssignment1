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
		ATTACK,
		DODGE,
		SWAP,
		CONFRONT,
		STOP1,
		
	};

	enum FSM_TWO
	{
		SWAP2,
		VOLUNTEER,
		AGGRESSIVE,
		NEXT, // This might be removed
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

		//void RenderState(Mesh* mesh, std::string text, Color color);

		Vector3 getRandPos();
		FSM_ONE FSM1;
		FSM_TWO FSM2;
		bool isVisible2D(const Vector3 &Position, float rotation, float FOV, const Vector3 &ObjectPosition);
		cAI* target;

		MessageBoard mbController;
	
	private:
		//Waypoints and states

		unsigned int wayPointIndex;
		int randNum;
		bool arrived;
		bool detected;
		bool missed;
		float offset;
		int startPoint;
		unsigned int ID;

		float probabilityIdle;
		float probabilityDodge;
		int timer;


		float probability;
		Vector3 vel;
		Vector3 nextPoint;
		

		//const double rangeofFOV = 100;
		//const float playerRadius = 0.25;
		//const float enemyRadius = 0.1f;
		//const float proximity = 0.4f;
		
	};
}
#endif