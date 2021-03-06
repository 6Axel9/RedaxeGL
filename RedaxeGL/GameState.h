#pragma once
#include <Deque>
#include <Map>
#include "CLight.h"
#include "CCamera.h"
#include "CWater.h"
#include "CTerrain.h"
#include "CInterface.h"
#include "CSkyBox.h"
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
	//============================================================= Inputs
	virtual void UpdateInputs(GLfloat DeltaTime) = 0;
	//============================================================= Reflection
	virtual void RenderReflection(GLboolean Shaded) = 0;
	//============================================================= Refraction
	virtual void RenderRefraction(GLboolean Shaded) = 0;
	//============================================================= Shadows
	virtual void RenderShadows(GLboolean Shaded) = 0;
	//============================================================= On Exit Stage
	virtual void OnExit() = 0;
	//==================================================== Terminate Game State
	virtual ~GameState();

	//==================================================== Handlers
	GLboolean& Status() { return status; }

protected:

	//==================================================== Properties
	GLfloat activeTime;
	GLboolean status;
};

