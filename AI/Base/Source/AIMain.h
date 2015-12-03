#ifndef AI_MAIN_H
#define AI_MAIN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "AI.h"
#include <irrKlang.h>

class AIMain : public SceneBase
{
public:
	AIMain();
	~AIMain();


	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO();
	void RenderObjects();
	GameObject* FetchGO();

	//Simple Variables
	
protected:

	//Might not use these Enums since we might use Integers to do 
	//the states
	

	
	//Protected Varian;es
	std::vector<GameObject*> m_goList;

private:
	
	//Variables here
	bool Perspec;
	float m_worldWidth;
	float m_worldHeight;

	//For Sound (do only if have time)
	irrklang::ISoundEngine* soundEngine;
};

#endif