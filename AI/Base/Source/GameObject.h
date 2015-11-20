#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_BALL2,
		GO_CUBE,
		GO_PILLAR,
		GO_GOALPOST,

		GO_PLAYER1,
		GO_PLAYER2,

		GO_ASTEROID,
		GO_SHIP,
		GO_ESHIP,
		GO_BULLET,
		GO_PLANET,
		GO_EBULLET,
		GO_POWERUP,
		GO_WALL,
		GO_MISSILE,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 rotation;
	bool active;
	float mass;
	float MAX_VEL;
	GameObject* homing;
	Vector3 normal;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif