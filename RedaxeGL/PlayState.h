#pragma once
#include "GameState.h"

class PlayState : public GameState
{
public:

	//============================================================= Initialize PlayState
	PlayState();
	//============================================================= On Enter Stage
	virtual void OnEnter();
	//============================================================= Update Stage
	virtual void Update(GLfloat DeltaTime);
	//============================================================= Render Stage
	virtual void Render(GLboolean WaterShader);
	//============================================================= On Exit Stage
	virtual void OnExit();
	//============================================================= Terminate PlayState
	~PlayState();

private:

	//============================================================= Properties
	CLight* Light;
	CCamera* Camera;
	CObject* Player;
	CInterface* GUI;
	CTerrain* Terrain;
	CWater* Water;
	CText* Text;
};

