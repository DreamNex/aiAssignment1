#ifndef AI_H
#define AI_H

#include "GameObject.h"
#include <vector>
#include "MessageBoard.h"


	
class cAI : public GameObject
{
	enum FSM_ONE
	{
	};

	enum FSM_TWO
	{
	};

public:
	cAI();
	virtual ~cAI();

	void init();
	void update(double dt);

	FSM_ONE FSM1;
	FSM_TWO FSM2;

private:



};
#endif