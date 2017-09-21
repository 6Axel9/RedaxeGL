#include "PlayState.h"

PlayState::PlayState()
{
	status = true;
	//============================================================= Create Camera
	Camera = new CCamera;
	//============================================================= Create Player
	Player = new CObject("Spaceship", "Spaceship", "Spaceship");
	//============================================================= Create Text
	Text   = new CText("Font", "Font", "Font");
}

void PlayState::OnEnter()
{
	//============================================================= Initialize Camera
	Camera->Initialize(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Initialize Player
	Player->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Write Text
	Text->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Yohooo");
}

void PlayState::Update(GLfloat DeltaTime)
{
	//============================================================= Update Camera
	Camera->Update(DeltaTime);
	//============================================================= Update Player
	Player->Update(DeltaTime);
	//============================================================= Update Text
	Text->Update(DeltaTime);
}

void PlayState::Render(GLboolean Shader)
{
	//============================================================= Render Camera 3D
	Camera->Render(true);
	//============================================================= Render Player
	Player->Render(true, true, false);
	//============================================================= Render Camera 2D
	Camera->Render(false);
	//============================================================= Render Text
	Text->Render(true, true, false);
}

void PlayState::OnExit()
{
	//============================================================= Terminate Camera
	Camera->Terminate();
	//============================================================= Terminate Player
	Player->Terminate();
	//============================================================= Terminate Text
	Text->Terminate();
}

PlayState::~PlayState()
{
	//============================================================= Destroy Camera
	delete Camera;
	//============================================================= Destroy Player
	delete Player;
	//============================================================= Destroy Text
	delete Text;
}
