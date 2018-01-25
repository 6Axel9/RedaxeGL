#include "PlayState.h"
#include "Engine.h"

PlayState::PlayState()
{
	//============================================================= Create Camera
	Camera = new CCamera;
	//============================================================= Create Light
	Light = new CLight("Point");
	//============================================================= Create Player
	Player = new CObject("Spaceship", "Spaceship", "Spaceship");
	//============================================================= Create Terrain
	Terrain = new CTerrain("Terrain", "Terrain", "Terrain");
	//============================================================= Create Water
	Water = new CWater("Water", "Water", "Water");
	//============================================================= Create Text
	Text   = new CText("Font", "Font", "Font");
}

void PlayState::OnEnter()
{
	//============================================================= Initialize Camera
	Camera->Initialize(glm::vec3(0.0f, 3.0f, -1.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Initialize Light
	Light->Initialize(glm::vec3(0.0f, 3.5f, 0.0f), glm::vec3(-90.0f,0.0f,0.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Enlighten Light
	//Light->Enlighten(glm::vec3(0.5f), glm::vec3(0.56f,0.28f,0.1f), glm::vec3(0.56f,0.33f,0.2f), 0.0f);
	Light->Enlighten(glm::vec3(0.3f, 0.35f, 0.3f), glm::vec3(0.375f, 0.35f, 0.3f), glm::vec3(0.275f, 0.25f, 0.2f), 0.0f);
	//============================================================= Initialize Player
	Player->Initialize(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Player
	Player->Materialize(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	//============================================================= Initialize Terrain
	Terrain->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Terrain
	Terrain->Materialize(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	//============================================================= Initialize Water
	Water->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Water
	Water->Materialize(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	//============================================================= Write Text
	Text->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Yohooo");
}

void PlayState::Update(GLfloat DeltaTime)
{
	if (Engine::Screen()->Key(GLFW_KEY_N))
	{
		Engine::SwitchGamestate(new PlayState,true);
	}
	if (Engine::Screen()->Key(GLFW_KEY_V))
	{
		Engine::SwitchGamestate(new PlayState, false);
	}
	if (Engine::Screen()->Key(GLFW_KEY_B))
	{
		Engine::ReturnGamestate();
	}
	//============================================================= Update Camera
	Camera->Update(DeltaTime);
	//============================================================= Update Light
	Light->Update(DeltaTime);
	//============================================================= Update Player
	Player->Update(DeltaTime);
	//============================================================= Update Terrain
	Terrain->Update(DeltaTime);
	//============================================================= Update Water
	Water->Update(DeltaTime);
	//============================================================= Update Text
	Text->Update(DeltaTime);
}

void PlayState::Render(GLboolean Shader)
{
	//============================================================= Render Camera 3D
	Camera->Render(true);
	//============================================================= Render Light
	Light->Render(false, false, true);
	//============================================================= Render Player
	Player->Render(true, true, true);
	//============================================================= Render Terrain
	Terrain->Render(true, true, true);
	//============================================================= Render Water
	Water->Render(false, false, true);
	//============================================================= Render Camera 2D
	Camera->Render(false);
	//============================================================= Render Text
	Text->Render(true, false, false);
}

void PlayState::OnExit()
{
	//============================================================= Terminate Camera
	Camera->Terminate();
	//============================================================= Terminate Light
	Light->Terminate();
	//============================================================= Terminate Player
	Player->Terminate();
	//============================================================= Terminate Terrain
	Terrain->Terminate();
	//============================================================= Terminate Water
	Water->Terminate();
	//============================================================= Terminate Text
	Text->Terminate();
}

PlayState::~PlayState()
{
	//============================================================= Destroy Camera
	delete Camera;
	//============================================================= Destroy Light
	delete Light;
	//============================================================= Destroy Player
	delete Player;
	//============================================================= Destroy Terrain
	delete Terrain;
	//============================================================= Destroy Water
	delete Water;
	//============================================================= Destroy Text
	delete Text;
}
