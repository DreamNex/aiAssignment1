#ifndef AI_H
#define AI_H

#include "GameObject.h"
#include <vector>
#include "Vector3.h"

namespace MyAI
{
	enum FSM_ONE
	{
		ATTACK,
		DODGE,
		RESET,
		STOP1,
	};

	enum FSM_TWO
	{
		PATROL,
		SCAN,
		IDLE,
		STOP2,
		
	};
	class cAI : public GameObject
	{
	public:
		cAI();
		virtual ~cAI();

		void init();
		void update(double dt);
		void FiniteStateM2();


		FSM_ONE FSM1;


		FSM_TWO FSM2;

	private:
		//Waypoints and states
		int state;
		int wayPointIndex;
		int randNum;
		bool arrived;
		float offset;
		float probability;
		const float playerRadius = 0.25;
		const float enemyRadius = 0.1f;
		const float proximity = 0.4f;
		const float AiSpeed = 0.0175f;
	};
}
#endif