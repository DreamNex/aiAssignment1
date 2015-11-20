#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
	Intervals = 0.f;
	Spawn = 0.f;
	AllowSpawn = true;
	NumAsters = 0;
	NumShips = 0;
	NumPower = 0;
	NumPlanets = 0;
	PowerUp = false;
	Rockets = false;
	Fire = false;
	level = 1;
	kill =0;
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	TimeTonext = Math::RandFloatMinMax(0,1000);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList

	for(int x = 0; x!= 100; ++x)
	{
		GameObject *go = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(go);
	}

	//Exercise 2b: Initialize m_lives and m_score

	m_lives = MAX_LIVES;
	m_score = START_SCORE;
	m_asteroids_hp = 2;

	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_ship = new GameObject(GameObject::GO_SHIP);

	m_ship->active = true;

	m_ship->scale.Set(1.5f,1.5f,1.5f);
	m_ship->pos.Set(m_worldWidth * 0.5f,m_worldHeight * 0.5f, 0);
	levelSwitch = false;
	GameState = MENU;
}

void SceneAsteroid::ResetGame()
{
	GameState = GAMEOVER;
}

void SceneAsteroid::FireBullet()
{
	GameObject *bullets = FetchGO();
	bullets->type = (GameObject::GO_BULLET);
	bullets->scale.Set(0.2,0.2,0.2);
	bullets->pos = m_ship->pos;
	if(m_ship->vel != 0)
	{
		bullets->vel = m_ship->vel.Normalize() * BULLET_SPEED;
	}

	else
	{
		bullets->vel = Vector3 (0,1,0) * BULLET_SPEED;
	}
}

void SceneAsteroid::FIREBULLETSEVERYWHERE()
{
	const int NumBullets = 8;
	for(int i = 0; i != NumBullets; i++)
	{
		Mtx44 rotation;
		rotation.SetToRotation(360/NumBullets ,0,0,1);
		m_ship->vel = rotation * m_ship->vel;
		FireBullet();
	}
}

bool SceneAsteroid::CollisionChecker(GameObject* d, GameObject* s)
{
	if((d->pos - s->pos).Length() < (d->scale.x + s->scale.x))
	{
		return true;
	}

	else
		return false;
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject * go = (GameObject *)*it;
		if(go->active == false)
		{
			go->active = true;
			go->pos.SetZero();
			go->vel.SetZero();

			return go;
		}
	}

	for(int i = 0; i <10; i++)
	{
		GameObject *go = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(go);

	}
	return m_goList[m_goList.size() - 1];
}

void SceneAsteroid::RestartGame()
{
	m_lives = MAX_LIVES;
	m_score = START_SCORE;
	m_ship->pos.Set(m_worldWidth * 0.5f,m_worldHeight * 0.5f, 0);

	for(std::vector<GameObject*>::iterator it = m_goList.begin(), end = m_goList.end(); it != end; ++it)
	{
		GameObject* asteroid;
		asteroid = *it;
		if(asteroid->active == true)
		{
			asteroid->active = false;
		}
	}
}

void SceneAsteroid::Levels(int temp, double dt)
{

	//==================================================

	if(NumPlanets != 2)
	{
		float const spacing = 15.f;
		++NumPlanets;
		GameObject *planets = FetchGO();
		planets->type = (GameObject::GO_PLANET);
		planets->scale.Set(4,4,4);
		planets->vel.SetZero();
		do
		{
			planets->pos.Set(Math::RandFloatMinMax((0 + planets->scale.x + spacing), m_worldWidth - planets->scale.x - spacing), Math::RandFloatMinMax((0 + planets->scale.y + spacing), m_worldHeight - planets->scale.y - spacing) ,0);
		} while ((planets->pos - m_ship->pos).Length() < 15);

	}


	if(Spawn >= 0.5 && AllowSpawn == true)
	{
		//float angle =  (rand() % 180) / Math::PI;
		int temp = Math::RandIntMinMax(1,3);
		//float angle = Math::RandFloatMinMax(0,2*Math::PI);

		if(temp == 1)
		{
			GameObject *asteroids = FetchGO();
			asteroids->type = (GameObject::GO_ASTEROID);
			asteroids->scale.Set(1,1,1);
			//asteroids->vel.Set(cos(angle),sin(angle),0);
			//asteroids->vel *= (rand() % 11) - 5;
			asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);

			do
			{
				asteroids->pos.Set(Math::RandFloatMinMax((0 + asteroids->scale.x + 3), m_worldWidth - asteroids->scale.x - 3), Math::RandFloatMinMax((0 + asteroids->scale.y + 3), m_worldHeight - asteroids->scale.y - 3) ,0);
			} while ((asteroids->pos - m_ship->pos).Length() < 15);

			NumAsters++;
		}

		else if(temp == 2)
		{
			GameObject *bigasters = FetchGO();
			bigasters->type = (GameObject::GO_ASTEROID);
			bigasters->scale.Set(2,2,2);
			bigasters->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);

			do
			{
				bigasters->pos.Set(Math::RandFloatMinMax((0 + bigasters->scale.x + 3), m_worldWidth - bigasters->scale.x - 3), Math::RandFloatMinMax((0 + bigasters->scale.y + 3), m_worldHeight - bigasters->scale.y - 3) ,0);
			} while ((bigasters->pos - m_ship->pos).Length() < 15);

			NumAsters += 2;
		}

		else if(temp == 3)
		{
			GameObject *eships = FetchGO();
			m_force2.Set(Math::RandIntMinMax(-100,100),Math::RandIntMinMax(-100,100),0);
			Vector3 acceleration2 = m_force2 * (1.f/ eships->mass);
			eships->type = (GameObject::GO_ESHIP);
			eships->scale.Set(1.5,1.5,1.5);
			eships->vel += acceleration2 * dt;
			do
			{
				eships->pos.Set(Math::RandFloatMinMax((0 + eships->scale.x + 3), m_worldWidth - eships->scale.x - 3), Math::RandFloatMinMax((0 + eships->scale.y + 3), m_worldHeight - eships->scale.y - 3) ,0);
			} while ((eships->pos - m_ship->pos).Length() < 15);

			++NumShips;
		}

		Spawn = 0;
	}

	//==================================================

	if(temp == 1)
	{
		if(!levelSwitch)
		{
			kill = 10;
			levelSwitch = true;
		}

		if(kill <= 0)
		{
			level++;
			levelSwitch = false;
		}
	}

	else if(temp == 2)
	{
		if(!levelSwitch)
		{
			kill = 15;
			levelSwitch = true;
		}

		if(kill <= 0)
		{
			level++;
			levelSwitch = false;
		}
	}

	else if (temp == 3)
	{		
		if(!levelSwitch)
		{
			kill = 20;
			levelSwitch = true;
		}

		if(NumPower == 0)
		{
			float const spacing = 15.f;
			NumPower++;
			GameObject *powerup = FetchGO();
			powerup->type = (GameObject::GO_POWERUP);
			powerup->scale.Set(1.5,1.5,1.5);
			powerup->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
			do
			{
				powerup->pos.Set(Math::RandFloatMinMax((0 + powerup->scale.x + spacing), m_worldWidth - powerup->scale.x - spacing), Math::RandFloatMinMax((0 + powerup->scale.y + spacing), m_worldHeight - powerup->scale.y - spacing) ,0);
			} while ((powerup->pos - m_ship->pos).Length() < 15);
		}

		if(kill <= 0)
		{
			level++;
		}
	}

	else if(temp >3)
	{
		GameState = WIN;

	}
}

void SceneAsteroid::Update(double dt)
{
	//Mouse Section
		static bool bLButtonState = false;
		if(!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;
		}
		else if(bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}
		static bool bRButtonState = false;
		if(!bRButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;
			std::cout << "RBUTTON DOWN" << std::endl;
		}
		else if(bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;
			std::cout << "RBUTTON UP" << std::endl;
		}

		switch(GameState)
		{
		case MENU:
			if(bLButtonState)
			{
				GameState = GAME;
			}
			break;

		case WIN:
			if(bLButtonState)
			{
				GameState = GAME;
				RestartGame();
			}
			break;

		case GAMEOVER:
			if(bLButtonState)
			{
				GameState = GAME;
				RestartGame();
			}
			break;

		case GAME:
			SceneBase::Update(dt);

		if(Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);
		}
		if(Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}
		m_force.SetZero();

		//Exercise 6: set m_force values based on WASD

		if(Application::IsKeyPressed('W'))
		{
			if(PowerUp == true)
				m_force.y = 200.f;
			else
				m_force.y = 100.f;
		}
		if(Application::IsKeyPressed('A'))
		{
			if(PowerUp == true)
				m_force.x = -200.f;
			else
				m_force.x = -100.f;
		}
		if(Application::IsKeyPressed('S'))
		{
			if(PowerUp == true)
				m_force.y = -200.f;
			else
				m_force.y = -100.f;
		}
		if(Application::IsKeyPressed('D'))
		{
			if(PowerUp == true)
				m_force.x = 200.f;
			else
				m_force.x = 100.f;
		}

		if(Application::IsKeyPressed(VK_UP))
		{
			m_ship->mass += 0.1f;
			if(m_ship->mass > 8)
			{
				m_ship->mass = 8;
			}
		}

		if(Application::IsKeyPressed(VK_DOWN))
		{
			m_ship->mass -= 0.1f;

			if(m_ship->mass < 0)
			{
				m_ship->mass = 1;
			}
		}

		//Spawning system======================================================================================
		Spawn += dt;

		Levels(level, dt);

		if(NumAsters > 10 && NumShips >2)
		{
			AllowSpawn = false;
		}

		else
		{
			AllowSpawn = true;
		}
		//==================================================================================================================

		// limit the spawn rate of bullets
		Intervals += dt;

		//Exercise 14: use a key to spawn a bullet
		if(Intervals > 0.2 && Application::IsKeyPressed(VK_SPACE))
		{
			Intervals = 0.f;

			if(PowerUp == true)
			{
				FIREBULLETSEVERYWHERE();
			}
			else
				FireBullet();
		}

		if(Application::IsKeyPressed('E') && Rockets == false)
		{
			Rockets = true;
			Intervals = 0.f;
			GameObject *missiles = FetchGO();
			missiles->type = (GameObject::GO_MISSILE);
			missiles->scale.Set(1,1,1);
			missiles->pos = m_ship->pos;
			missiles->homing = NULL;
			if(m_ship->vel != 0)
			{
				missiles->vel = m_ship->vel.Normalize() * MISSILE_SPEED;
			}

			else
			{
				missiles->vel = Vector3(0,1,0) * MISSILE_SPEED;
			}

		}


		//Physics Simulation Section

		//Exercise 7: Update ship's velocity based on m_force
		//f = m * a
		//a = f /  m
		Vector3 accel = m_force * (1.f/ m_ship->mass);
		m_ship->vel += accel * dt;

		//Exercise 10: Limit the velocity

		if(m_ship->vel.Length() > MAX_SPEED)
		{
			((m_ship->vel.Normalize()) *= MAX_SPEED);
		}

		m_ship->pos += m_ship->vel * dt;


		//Exercise 9: wrap ship position if it leaves screen

		if(m_ship->pos.x > m_worldWidth)
		{
			m_ship->pos.x = 0;
		}
		else if (m_ship->pos.x < 0)
		{
			m_ship->pos.x = m_worldWidth;
		}

		else if( m_ship->pos.y > m_worldHeight)
		{
			m_ship->pos.y = 0;
		}

		else if(m_ship->pos.y < 0)
		{
			m_ship->pos.y = m_worldHeight;
		}

		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if(!go->active)
			{
				continue;
			}
			go->pos += go->vel * dt;
			if(go->type == GameObject::GO_MISSILE)
			{
				if(go->homing != NULL)
				{
					if(go->homing->active == false)
					{
						go->homing = NULL;
					}
				}

				if(go->homing == NULL)
				{
					GameObject* nearestaster;
					nearestaster = NULL;

					for(std::vector<GameObject*>::iterator it = m_goList.begin(), end = m_goList.end(); it != end; ++it)
					{
						GameObject *asteroid;
						asteroid = *it;
						if(!(asteroid->type == GameObject::GO_ASTEROID || asteroid->type == GameObject::GO_ESHIP))
						{
							continue;
						}
						if(asteroid->active == true)
						{
							if(nearestaster == NULL)
							{
								nearestaster = asteroid;
							}

							else if ((asteroid->pos - go->pos).Length() < (nearestaster->pos - go->pos).Length())
							{
								nearestaster = asteroid;
							}
						}
					}

					go->homing = nearestaster;
				}

				if(go->homing != NULL)
				{
					Vector3 direction;
					direction = go->homing->pos - go->pos;

					if(!direction.IsZero())
					{
						go->vel = direction.Normalized() * MISSILE_SPEED;
					}
				}


				for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					GameObject *go3 = (GameObject *)*it;

					if(go3->active)
					{
						if(go3->type == GameObject::GO_ASTEROID || go3->type == GameObject::GO_ESHIP)
						{
							if(CollisionChecker(go3,go) == true) //missile collision
							{
								Rockets = false;
								go3->active = false;
								go->active = false;
								m_score++;
								kill--;
								NumAsters--;
							}
						}
					}
				}
			}

			//ENEMY BULLET COLLISION//
			if(go->type == GameObject::GO_EBULLET)
			{
				if(go->active)
				{
					if(CollisionChecker(m_ship,go))
					{
						m_ship->pos.Set(m_worldWidth * 0.5f,m_worldHeight * 0.5f, 0);
						m_ship->vel.SetZero();
						--m_lives;
						go->active = false;
						Fire = true;
						NumAsters--;
					}

					for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
					{
						GameObject *go2 = (GameObject *)*it; //asteroid

						if(go2->type == GameObject::GO_ASTEROID && go2->active)
						{
							if(CollisionChecker(go,go2) == true)
							{
								go2->scale -= Vector3(1,1,1);									//using scale as health
								go2->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
								go->active = false;
								m_score += 2;
								NumAsters--;

								if(go2->scale.x <= 0)
								{
									go2->active = false;
									m_score += 2;
									NumAsters--;
								}

								else
								{
									GameObject *asteroids = FetchGO();

									asteroids->type = (GameObject::GO_ASTEROID);
									asteroids->scale = go2->scale;
									asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
									asteroids->pos.Set(go2->pos.x,go2->pos.y,go2->pos.z);

								}

								break;
							}
						}
					}
				}
			}

			//ENEMY SHIP COLLISION//
			if(go->type == GameObject::GO_ESHIP)
			{
				if(go->active)
				{

					//Set bullet
					if(Fire == true)
					{
						Fire = false;
						GameObject *ebullets = FetchGO();
						ebullets->type = (GameObject::GO_EBULLET);
						ebullets->scale.Set(0.2,0.2,0.2);
						ebullets->pos = go->pos;
						if(go->vel != 0)
						{
							ebullets->vel = go->vel.Normalize() * BULLET_SPEED;
						}

						else
						{
							ebullets->vel = Vector3 (0,1,0) * BULLET_SPEED;
						}

						if(ebullets->pos.x > m_worldWidth || ebullets->pos.y > m_worldHeight || ebullets->pos.x < 0 || go->pos.y < 0)
						{
							ebullets->active = false;
						}
					}


					if(CollisionChecker(m_ship,go) == true)
					{
						m_ship->pos.Set(m_worldWidth * 0.5f,m_worldHeight * 0.5f, 0);
						m_ship->vel.SetZero();
						--m_lives;
						--kill;
						go->active = false;
						NumAsters--;
					}

					if(m_lives <= 0)
					{
						ResetGame();
					}
				}

				if(go->pos.x > m_worldWidth)
				{
					go->pos.x = 0;
				}
				else if (go->pos.x < 0)
				{
					go->pos.x = m_worldWidth;	
				}

				if( go->pos.y > m_worldHeight)
				{
					go->pos.y = 0;
				}

				else if(go->pos.y < 0)
				{
					go->pos.y = m_worldHeight;
				}
			}

			//ASTEROID COLLISION//
			if(go->type == GameObject::GO_ASTEROID)
			{
				if(go->active)
				{
					//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check

					if(CollisionChecker(m_ship,go) == true)
					{
						m_ship->pos.Set(m_worldWidth * 0.5f,m_worldHeight * 0.5f, 0);
						m_ship->vel.SetZero();
						--m_lives;
						--kill;
						go->active = false;
						NumAsters--;

						if(go->scale.x <= 0)
						{
							go->active = false;
							m_score += 2;
							NumAsters--;
						}

						else
						{
							GameObject *asteroids = FetchGO();

							asteroids->type = (GameObject::GO_ASTEROID);
							asteroids->scale = go->scale;
							asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
							asteroids->pos.Set(go->pos.x,go->pos.y,go->pos.z);

						}

						break;
					}

					if(m_lives <= 0)
					{
						ResetGame();
					}


					for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
					{
						GameObject* go3;
						go3 = *it;

						if(go3->type == GameObject::GO_EBULLET) //ENEMY BULLET COLLIDE WITH THE ASTEROID
						{
							if(CollisionChecker(go3,go))
							{
								if(go->scale.x <= 0)
								{
									go->active = false;
									m_score += 2;
									NumAsters--;
								}

								else
								{
									GameObject *asteroids = FetchGO();

									asteroids->type = (GameObject::GO_ASTEROID);
									asteroids->scale = go->scale;
									asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
									asteroids->pos.Set(go->pos.x,go->pos.y,go->pos.z);

								}

								break;
							}
						}

					}
				}


				//Exercise 13: asteroids should wrap around the screen like the ship

				if(go->pos.x > m_worldWidth)
				{
					go->pos.x = 0;
				}
				else if (go->pos.x < 0)
				{
					go->pos.x = m_worldWidth;	
				}

				if( go->pos.y > m_worldHeight)
				{
					go->pos.y = 0;
				}

				else if(go->pos.y < 0)
				{
					go->pos.y = m_worldHeight;
				}

			}

			//POWERUP COLLISION//
			if(go->type == GameObject::GO_POWERUP)
			{

				if(CollisionChecker(go,m_ship) == true)
				{
					PowerUp = true;
					go->active = false;
				}


				if(go->pos.x > m_worldWidth - go->scale.x || go->pos.x < 0 + go->scale.x)
				{
					go->vel.x = -go->vel.x;
				}

				if( go->pos.y > m_worldHeight - go->scale.y || go->pos.y < 0 + go->scale.y)
				{
					go->vel.y = -go->vel.y;
				}
			}

			if(go->type == GameObject::GO_PLANET)
			{

				if(CollisionChecker(go,m_ship) == true)
				{

					m_ship->pos.Set(m_worldWidth * 0.5f,m_worldHeight * 0.5f, 0);
					m_ship->vel.SetZero();
					--m_lives;				
					NumAsters--;

					if(m_lives <= 0)
					{
						ResetGame();
					}

				}


				for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					GameObject *go3 = (GameObject *)*it;

					if(go3->active)
					{
						if(go3->type == GameObject::GO_ASTEROID || go3->type == GameObject::GO_ESHIP || go3->type == GameObject::GO_MISSILE)
						{
							if(CollisionChecker(go3,go) == true) //missile collision
							{
								Rockets = false;
								go3->active = false;
								NumAsters--;
							}
						}

						if(go3->type == GameObject::GO_POWERUP)
						{
							if(go3->active)
							{
								if(CollisionChecker(go3,go) == true)
								{
									go3->vel.x = -go3->vel.x;
									go3->vel.y = -go3->vel.y;
								}
							}
						}
					}
				}
			}

			//Exercise 16: unspawn bullets when they leave screen

			if(go->type == GameObject::GO_BULLET)
			{
				if(go->pos.x > m_worldWidth || go->pos.y > m_worldHeight || go->pos.x < 0 || go->pos.y < 0)
				{
					go->active = false;
				}

				//Exercise 18: collision check between GO_BULLET and GO_ASTEROID

				for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					GameObject *go2 = (GameObject *)*it; //asteroid

					if(go2->type == GameObject::GO_ASTEROID && go2->active)
					{
						if(CollisionChecker(go,go2) == true)
						{
							go2->scale -= Vector3(1,1,1);									//using scale as health
							go2->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
							go->active = false;
							m_score += 2;
							NumAsters--;

							if(go2->scale.x <= 0)
							{
								go2->active = false;
								--kill;
								m_score += 2;
								NumAsters--;
							}

							else
							{
								GameObject *asteroids = FetchGO();

								asteroids->type = (GameObject::GO_ASTEROID);
								asteroids->scale = go2->scale;
								asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
								asteroids->pos.Set(go2->pos.x,go2->pos.y,go2->pos.z);

							}

							break;
						}
					}

					if(go2->type == GameObject::GO_ESHIP && go2->active)
					{
						if(CollisionChecker(go2,go))
						{
							go2->active = false;
							m_score += 3;
							--kill;
							NumShips--;
						}
					}

				}
			}
		}
		break;
	}	
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	switch(go->type)
	{

	case GameObject::GO_MISSILE:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
			float theta = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
			modelStack.Rotate(theta,0,0,1);
			modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
			RenderMesh(meshList[GEO_MISSILE], false);
			modelStack.PopMatrix();
		}
		break;

	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
		modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_EBULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
		modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
		RenderMesh(meshList[GEO_EBULLETS], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
			float theta = Math::RadianToDegree(atan2(m_ship->vel.y, m_ship->vel.x));
			modelStack.Rotate(theta,0,0,1);
			modelStack.Scale(m_ship->scale.x,m_ship->scale.y,m_ship->scale.z);
			RenderMesh(meshList[GEO_SHIP], false);
			modelStack.PopMatrix();
		}
		break;

	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
		modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_ESHIP:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
			float theta = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
			modelStack.Rotate(theta,0,0,1);
			modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
			RenderMesh(meshList[GEO_ESHIP], false);
			modelStack.PopMatrix();
		}
		break;

	case GameObject::GO_PLANET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLANETS], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_POWERUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
		RenderMesh(meshList[GEO_POWERUP],false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAsteroid::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	float Zaxis = 0.f;
	const float increment = 0.001;

	switch(GameState)
	{
	case GAME:
		{
			//RenderMesh(meshList[GEO_AXES], false);

			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, Zaxis);
			modelStack.Scale(m_worldWidth * 0.5,m_worldHeight * 0.5, 0);
			RenderMesh(meshList[GEO_BACKGROUND],false);
			modelStack.PopMatrix();

			Zaxis += increment;

			for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if(go->active)
				{
					go->pos.z = Zaxis;
					Zaxis += increment;
					RenderGO(go);
				}
			}

			m_ship->pos.z = Zaxis;
			RenderGO(m_ship);


			//On screen text

			//Exercise 5a: Render m_lives, m_score
			std::ostringstream ss3;
			ss3 << "Lives: " << m_lives;
			RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(),Color(1,1,1),2.5,2,55);

			std::ostringstream ss4;
			ss4 << "Score: " << m_score;
			RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(),Color(1,1,1),2.5,2,53);
			//Exercise 5b: Render position, velocity & mass of ship

			std::ostringstream ss5;
			ss5.precision(3);
			ss5 << "Pos: " << m_ship->pos;
			RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(),Color(1,1,1),2.5,2,51);

			std::ostringstream ss6;
			ss6.precision(1);
			ss6 << "Mass: " << m_ship->mass;
			RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(),Color(1,1,1),2.5,2,49);

			std::ostringstream ss7;
			ss7.precision(3);
			ss7 << "Vel: " << m_ship->vel.Length();
			RenderTextOnScreen(meshList[GEO_TEXT], ss7.str(), Color(1, 1, 1), 2.5, 0, 47);

			std::ostringstream ss2;
			ss2.precision(3);
			ss2 << "Speed: " << m_speed;
			RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 6);

			std::ostringstream ss;
			ss.precision(5);
			ss << "FPS: " << fps;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

			std::ostringstream sss;
			sss.precision(3);
			sss << "Kills Left: " << kill;
			RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 3, 0, 9);

			std::ostringstream ss9;
			ss9.precision(2);
			ss9 << "Level: " << level;
			RenderTextOnScreen(meshList[GEO_TEXT], ss9.str(), Color(0, 1, 0), 3, 30, 55);
			break;

		}

	case WIN:
		{

			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, Zaxis);
			modelStack.Scale(m_worldWidth * 0.5,m_worldHeight * 0.5, 0);
			RenderMesh(meshList[GEO_WIN],false);
			modelStack.PopMatrix();

			break;
		}

	case GAMEOVER:
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, Zaxis);
			modelStack.Scale(m_worldWidth * 0.5,m_worldHeight * 0.5, 0);
			RenderMesh(meshList[GEO_GAMEOVER],false);
			modelStack.PopMatrix();
			break;
		}

	case MENU:
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, Zaxis);
			modelStack.Scale(m_worldWidth * 0.5,m_worldHeight * 0.5, 0);
			RenderMesh(meshList[GEO_START],false);
			modelStack.PopMatrix();
			break;
		}
	}
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
