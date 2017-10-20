#include "PlayState.h"

PlayState::PlayState()
{
	status = true;
	//============================================================= Create Light
	Light = new CLight;
	//============================================================= Create Camera
	Camera = new CCamera;
	//============================================================= Create Player
	Player = new CObject("Spaceship", "Spaceship", "Spaceship");
	//============================================================= Create Text
	Text   = new CText("Font", "Font", "Font");
}

void PlayState::OnEnter()
{
	//============================================================= Initialize Light
	Light->Initialize(glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Enlighten Light
	Light->Enlighten(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 10.0f);
	//============================================================= Initialize Camera
	Camera->Initialize(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Initialize Player
	Player->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Player
	Player->Materialize(glm::vec3(0.01f), glm::vec3(0.01f), glm::vec3(0.01f), 1.0f);
	//============================================================= Write Text
	Text->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Yohooo");
}

void PlayState::Update(GLfloat DeltaTime)
{
	//============================================================= Update Light
	Light->Update(DeltaTime);
	//============================================================= Update Camera
	Camera->Update(DeltaTime);
	//============================================================= Update Player
	Player->Update(DeltaTime);
	//============================================================= Update Text
	Text->Update(DeltaTime);
}

void PlayState::Render(GLboolean Shader)
{
	//============================================================= Render Light
	Light->Render(true, false, true);
	//============================================================= Render Camera 3D
	Camera->Render(true);
	//============================================================= Render Player
	Player->Render(true, false, true);
	//============================================================= Render Camera 2D
	Camera->Render(false);
	//============================================================= Render Text
	Text->Render(true, false, false);
}

void PlayState::OnExit()
{
	//============================================================= Terminate Light
	Light->Terminate();
	//============================================================= Terminate Camera
	Camera->Terminate();
	//============================================================= Terminate Player
	Player->Terminate();
	//============================================================= Terminate Text
	Text->Terminate();
}

PlayState::~PlayState()
{
	//============================================================= Destroy Light
	delete Light;
	//============================================================= Destroy Camera
	delete Camera;
	//============================================================= Destroy Player
	delete Player;
	//============================================================= Destroy Text
	delete Text;
}
