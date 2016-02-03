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
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_BACKGROUND,
		NUM_GEOMETRY,
	};

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Vector3 WayPoints[8];

	void RenderGO();
	void RenderObjects();
	GameObject* FetchGO();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const int size = 1, const int x = 0, const int y = 0, const bool rotate = false, const bool flip = false);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

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
	float rotateAngle;
	int state;
	int state2;
	int respawntime;

	//For Sound (do only if have time)
	irrklang::ISoundEngine* soundEngine;
};

#endif