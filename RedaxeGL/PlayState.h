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
	virtual void Render(GLboolean Shader);
	//============================================================= On Exit Stage
	virtual void OnExit();
	//============================================================= Terminate PlayState
	~PlayState();

private:

	//============================================================= Properties
	CText* Text;
	CCamera* Camera;
	CObject* Player;
};

