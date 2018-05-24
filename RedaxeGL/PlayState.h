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

	//============================================================= Properties
	CCamera*	Camera;
	CSkyBox*	Skybox;
	CLight*		Light;
	CTerrain*	Terrain;
	CWater*		Water;
	CInterface* Player;

	std::deque<BModel*> TargetObjects;
	std::deque<std::pair<CInterface*, CText*>> TargetProperties;
	std::deque<std::pair<CInterface*, CText*>> TargetButtons;

	std::pair<CInterface*, CText*> Ambient;
	std::pair<CInterface*, CText*> Diffuse;
	std::pair<CInterface*, CText*> Specular;

	std::pair<CInterface*, CText*> Red;
	std::pair<CInterface*, CText*> Green;
	std::pair<CInterface*, CText*> Blue;
};