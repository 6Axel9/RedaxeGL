#include "PlayState.h"
#include "Engine.h"

PlayState::PlayState()
{
	//==================================================== Enable Alpha Blend
	Engine::Screen()->EnableBlending();
	//==================================================== Disable Clip Distance
	Engine::Screen()->DisableClipping();
	//==================================================== Disable FaceCulling
	Engine::Screen()->DisableFaceCulling();
	//============================================================= Active Time
	activeTime = 0;

	//============================================================= Create Camera
	Camera = new CCamera;
	//============================================================= Create Light
	Light = new CLight("Point");
	//============================================================= Create Player
	Player = new CObject("Spaceship", "Spaceship", "None");
	//============================================================= Create Terrain
	Terrain = new CTerrain("Terrain", "Terrain", "None");
	//============================================================= Create Water
	Water = new CWater("Water", "Water", "None");
	//============================================================= Create GUI
	GUI = new CInterface("Box", "Interface", "None");
	//============================================================= Create Text
	Text = new CText("Font", "Font", "None");
}

void PlayState::OnEnter()
{
	//============================================================= Initialize Camera
	Camera->Initialize(glm::vec3(0.0f, 5.0f, -1.0f), glm::vec3(0.0f), 100.0f);

	//============================================================= Initialize Light
	Light->Initialize(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(-90.0f,0.0f,0.0f), glm::vec3(0.0f), 100.0f);
	//============================================================= Enlighten Light
	Light->Enlighten(glm::vec3(0.45f, 0.45f, 0.4f), glm::vec3(0.8f,0.75f,0.5f), glm::vec3(0.3f), 150.0f);

	//============================================================= Initialize Player
	Player->Initialize(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Player
	Player->Materialize(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f), 1.0f);

	//============================================================= Initialize Terrain
	Terrain->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Terrain
	Terrain->Materialize(glm::vec3(1.0f, 0.8f, 0.6f), glm::vec3(0.7f), glm::vec3(0.5f), 1.0f);

	//============================================================= Initialize Water
	Water->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 100.0f);
	//============================================================= Materialize Water
	Water->Materialize(glm::vec3(0.6f, 0.8f, 1.0f), glm::vec3(0.3f), glm::vec3(0.5f), 1.0f);

	//============================================================= Write GUI
	GUI->Initialize(glm::vec3(275.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(250.0f, 100.0f, 1.0f));

	//============================================================= Write Text
	Text->Initialize(glm::vec3(275.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Credits to ME :P");
}

void PlayState::Update(GLfloat DeltaTime)
{
	activeTime += DeltaTime;
	//============================================================= Send Delta Time 
	glUniform1f(Engine::Shader()->UniformsID()["DeltaTime"], DeltaTime);
	glUniform1f(Engine::Shader()->UniformsID()["ElapsedTime"], activeTime);
	//============================================================= State Inputs
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

void PlayState::Render(GLboolean Shaded)
{
	//==================================================== Reflection
	RenderReflection(Shaded);
	//==================================================== Refraction
	RenderRefraction(Shaded);
	//==================================================== Shadows
	RenderShadows(Shaded);

	//==================================================== Restore Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//==================================================== Enable Alpha Blend
	Engine::Screen()->EnableBlending();
	//==================================================== Disable Clip Distance
	Engine::Screen()->DisableClipping();
	//==================================================== Disable FaceCulling
	Engine::Screen()->DisableFaceCulling();
	//==================================================== Clear Current Buffer
	Engine::Screen()->ClearScreen();

	//============================================================= Render Camera 3D
	Camera->Render(true, false);
	//============================================================= Update Light
	Light->Render(false, false, false, Shaded);
	//============================================================= Render Player
	Player->Render(true, true, true, Shaded);
	//============================================================= Render Terrain
	Terrain->Render(true, true, true, Shaded);
	//============================================================= Render Water
	Water->Render(true, true, true, Shaded);
	
	//============================================================= Render Camera 2D
	Camera->Render(false, false);
	//============================================================= Render GUI
	GUI->Render(Shaded, false, false, false);
	//============================================================= Render Text
	Text->Render(Shaded, false, false, false);

	//==================================================== Swap Front/Back Buffer
	Engine::Screen()->SwapBuffers();
}

void PlayState::RenderReflection(GLboolean Shaded)
{
	//==================================================== Bind Water Reflection
	glBindFramebuffer(GL_FRAMEBUFFER, Water->Reflection());
	//==================================================== Enable Clip Distance
	Engine::Screen()->EnableClipping();
	//==================================================== Enable FaceCulling
	Engine::Screen()->EnableFaceCulling();
	//==================================================== Clear Current Buffer
	Engine::Screen()->ClearScreen();

	//============================================================= Render Camera 3D
	Camera->Render(true, true);
	//============================================================= Update Light
	Light->Render(false, false, false, Shaded);
	//============================================================= Render Player
	Player->Render(true, true, true, Shaded);
	//============================================================= Render Terrain
	Terrain->Render(true, true, true, Shaded);
}

void PlayState::RenderRefraction(GLboolean Shaded)
{
	//==================================================== Bind Water Refraction
	glBindFramebuffer(GL_FRAMEBUFFER, Water->Refraction());
	//==================================================== Enable Clip Distance
	Engine::Screen()->EnableClipping();
	//==================================================== Enable FaceCulling
	Engine::Screen()->EnableFaceCulling();
	//==================================================== Clear Current Buffer
	Engine::Screen()->ClearScreen();

	//============================================================= Render Camera 3D
	Camera->Render(true, false);
	//============================================================= Update Light
	Light->Render(false, false, false, Shaded);
	//============================================================= Render Player
	Player->Render(true, true, true, Shaded);
	//============================================================= Render Terrain
	Terrain->Render(true, true, true, Shaded);
}

void PlayState::RenderShadows(GLboolean Shaded)
{
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
