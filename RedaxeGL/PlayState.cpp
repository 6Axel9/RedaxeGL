#include "PlayState.h"
#include "Engine.h"

PlayState::PlayState()
{
	//============================================================= Create Camera
	Camera = new CCamera;
	//============================================================= Create Light
	Light = new CLight("Point");
	//============================================================= Create Player
	Player = new CObject("Spaceship", "Spaceship", "None");
	//============================================================= Create Terrain
	Terrain = new CTerrain("Terrain", "Terrain", "None");
	//============================================================= Create Water
	Water = new CWater("Water", "None", "None");
	//============================================================= Create GUI
	GUI = new CInterface("Box", "Water", "None");
	//============================================================= Create Text
	Text = new CText("Font", "Font", "None");
}

void PlayState::OnEnter()
{
	//============================================================= Initialize Camera
	Camera->Initialize(glm::vec3(0.0f, 3.0f, -1.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Initialize Light
	Light->Initialize(glm::vec3(0.0f, 3.5f, 0.0f), glm::vec3(-90.0f,0.0f,0.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Enlighten Light
	Light->Enlighten(glm::vec3(0.5f, 0.4f, 0.3f), glm::vec3(0.4f, 0.5f, 0.3f), glm::vec3(0.3f, 0.2f, 0.2f), 0.0f);
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
	//============================================================= Write GUI
	GUI->Initialize(glm::vec3(280.0f,210.0f,0.0f), glm::vec3(0.0f), glm::vec3(240.0f,180.0f,1.0f));
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
	//============================================================= Update GUI
	GUI->Update(DeltaTime);
	//============================================================= Update Text
	Text->Update(DeltaTime);
}

void PlayState::Render(GLboolean WaterShader)
{
	if (WaterShader)
	{
		//==================================================== Bind Water Reflection
		glBindFramebuffer(GL_FRAMEBUFFER, Engine::Loader()->FrameBuffers("Water")[0]);
		//==================================================== Clear Current Buffer
		Engine::Screen()->ClearScreen();
		//============================================================= Render Camera 3D
		Camera->Render(true);
		//============================================================= Render Light
		Light->Render(false, false, true);
		//============================================================= Render Player
		Player->Render(true, true, true);
		//============================================================= Render Terrain
		Terrain->Render(true, true, true);
	}
	else
	{
		//==================================================== Unbind Water Reflection
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//==================================================== Clear Current Buffer
		Engine::Screen()->ClearScreen();
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
		//============================================================= Render GUI
		GUI->Render(true, false, false);
		//============================================================= Render Text
		Text->Render(true, false, false);
		//==================================================== Swap Front/Back Buffer
		Engine::Screen()->SwapBuffers();
	}
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
	//============================================================= Terminate GUI
	GUI->Terminate();
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
	//============================================================= Destroy GUI
	delete GUI;
	//============================================================= Destroy Text
	delete Text;
}
