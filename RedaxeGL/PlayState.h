#pragma once
#include "GameState.h"

class PlayState : public GameState
{
public:

	//============================================================= Initialize PlayState
	PlayState();
	//============================================================= On Enter Stage
	void OnEnter();
	//============================================================= Update Stage
	void Update(GLfloat DeltaTime);
	//============================================================= Render Stage
	void Render(GLboolean Shaded);
	//============================================================= Inputs
	void UpdateInputs(GLfloat DeltaTime);
	//============================================================= Reflection
	void RenderReflection(GLboolean Shaded);
	//============================================================= Refraction
	void RenderRefraction(GLboolean Shaded);
	//============================================================= Shadows
	void RenderShadows(GLboolean Shaded);
	//============================================================= On Exit Stage
	void OnExit();
	//============================================================= Terminate PlayState
	virtual ~PlayState();

private:

	
	//============================================================= Target
	void Link(BModel* Component) { DemoTarget = Component; }
	//============================================================= Properties
	CCamera*	Camera;
	CSkyBox*	Skybox;
	CLight*		Light;
	CTerrain*	Terrain;
	CWater*		Water;
	CInterface* Player;
	CInterface* Interface;
	CText*		Text;
	//============================================================= Modifiers
	BModel*	  DemoTarget;
	GLboolean DiffuseMAP;
	GLboolean SpecularMAP;
	GLboolean NormalMAP;
};