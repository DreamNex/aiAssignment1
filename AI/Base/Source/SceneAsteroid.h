#ifndef SCENE_ASTEROID_H
#define SCENE_ASTEROID_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAsteroid : public SceneBase
{
	static const int MAX_SPEED = 50;
	static const int BULLET_SPEED = 30;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;
	static const int MAX_LIVES = 3;
	static const int START_SCORE = 0;

public:
	SceneAsteroid();
	~SceneAsteroid();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void ResetGame();
	void RestartGame();
	bool CollisionChecker(GameObject* d, GameObject* s);
	void FIREBULLETSEVERYWHERE();
	void FireBullet();
	void Levels(int temp, double dt);
	void SSystem(double dt); // spawn system

	GameObject* FetchGO();
	float Intervals;
	bool Fire;
	float Spawn;
	int NumAsters;
	int NumShips;
	int NumPlanets;
	int NumPower;
	int level;
	int kill;
	bool PowerUp;
	bool AllowSpawn;
	bool Rockets;

protected:
	enum STATE_GAME
	{
		MENU,
		GAME,
		GAMEOVER,
		WIN,
	};
	STATE_GAME GameState;
	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	float TimeTonext;

	GameObject *m_ship;

	Vector3 m_force;
	Vector3 m_force2;

	int m_objectCount;
	int m_lives;
	int m_score;
	int m_kills;
	int m_asteroids_hp;
	bool levelSwitch;
};

#endif