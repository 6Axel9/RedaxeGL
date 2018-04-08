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
	virtual void Render(GLboolean Shaded);
	//============================================================= Reflection
	virtual void RenderReflection(GLboolean Shaded);
	//============================================================= Refraction
	virtual void RenderRefraction(GLboolean Shaded);
	//============================================================= Shadows
	virtual void RenderShadows(GLboolean Shaded);
	//============================================================= On Exit Stage
	virtual void OnExit();
	//============================================================= Terminate PlayState
	~PlayState();

private:

	//============================================================= Properties
	CLight* Light;
	CCamera* Camera;
	CObject* Player;
	CSkyBox* Skybox;
	CInterface* GUI;
	CTerrain* Terrain;
	CWater* Water;
	CText* Text;
};