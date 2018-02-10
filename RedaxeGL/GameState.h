#pragma once
#include "CLight.h"
#include "CCamera.h"
#include "CWater.h"
#include "CTerrain.h"
#include "CInterface.h"
#include "CObject.h"
#include "CText.h"

class GameState
{
public:
	
	//==================================================== Initialize Game State
	GameState();
	//============================================================= On Enter Stage
	virtual void OnEnter() = 0;
	//============================================================= Update Stage
	virtual void Update(GLfloat DeltaTime) = 0;
	//============================================================= Render Stage
	virtual void Render(GLboolean Shader) = 0;
	//============================================================= On Exit Stage
	virtual void OnExit() = 0;
	//==================================================== Terminate Game State
	virtual~GameState();

	//==================================================== Handlers
	GLboolean& Status() { return status; }

protected:

	//==================================================== Properties
	GLboolean status;
};

