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
	//============================================================= Create Skybox
	Skybox = new CSkyBox("Cube", "Skybox", "None");
	//============================================================= Create Terrain
	Terrain = new CTerrain("Terrain", "Terrain", "None");
	//============================================================= Create Water
	Water = new CWater("Water", "Water", "None");
	//============================================================= Create Player
	Player = new CInterface("Box", "Cockpit", "None");
	//============================================================= Create GUI
	Interface = new CInterface("Box", "Interface", "None");
	//============================================================= Create Text
	Text = new CText("Font", "Font", "None");
}

void PlayState::OnEnter()
{
	//============================================================= Initialize Camera
	Camera->Initialize(glm::vec3(0.0f, 5.0f, -1.0f), glm::vec3(0.0f), 50.0f);
	
	//============================================================= Initialize Skybox
	Skybox->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	//============================================================= Initialize Light
	Light->Initialize(glm::vec3(25.0f, 1000.0f, 1000.0f), glm::vec3(-90.0f,0.0f,0.0f), glm::vec3(0.0f));
	//============================================================= Enlighten Light
	Light->Enlighten(glm::vec3(0.45f, 0.45f, 0.4f), glm::vec3(0.8f,0.75f,0.5f), glm::vec3(0.3f), 10000.0f);
	
	//============================================================= Initialize Terrain
	Terrain->Initialize(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	//============================================================= Materialize Terrain
	Terrain->Materialize(glm::vec3(0.5f), glm::vec3(0.6f,0.6f,0.3f), glm::vec3(0.2f), 1.0f);

	//============================================================= Initialize Water
	Water->Initialize(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 7.0f);
	//============================================================= Materialize Water
	Water->Materialize(glm::vec3(0.6f, 0.8f, 1.0f), glm::vec3(0.3f), glm::vec3(0.5f), 1.0f);

	//============================================================= Initialize Player
	Player->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(Engine::Screen()->Size(), 1.0f));

	//============================================================= Initialize Interface
	Interface->Initialize(glm::vec3(275.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(250.0f, 100.0f, 1.0f));

	//============================================================= Write Text
	Text->Initialize(glm::vec3(275.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Credits to ME :P");
}



void PlayState::Update(GLfloat DeltaTime)
{
	activeTime += DeltaTime;
	//============================================================= Send Delta Time 
	glUniform1f(Engine::Shader()->UniformsID()["DeltaTime"], DeltaTime);
	glUniform1f(Engine::Shader()->UniformsID()["ElapsedTime"], activeTime);
	
	//============================================================= Inputs
	UpdateInputs(DeltaTime);
	//============================================================= Update Camera
	Camera->Update(DeltaTime);
	//============================================================= Update Skybox
	Skybox->Update(DeltaTime);
	//============================================================= Update Light
	Light->Update(DeltaTime);
	//============================================================= Update Terrain
	Terrain->Update(DeltaTime);
	//============================================================= Update Water
	Water->Update(DeltaTime);
	//============================================================= Update Player
	Player->Update(DeltaTime);
	//============================================================= Update Interface
	Interface->Update(DeltaTime);
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
	//============================================================= Render Skybox
	Skybox->Render(Shaded, false, false, false);
	//============================================================= Render Light
	Light->Render(false, false, false, Shaded);
	//============================================================= Render Terrain
	Terrain->Render(true, true, true, Shaded);
	//============================================================= Render Water
	Water->Render(true, true, true, Shaded);

	//============================================================= Render Camera 2D
	Camera->Render(false, false);
	//============================================================= Render Player
	Player->Render(Shaded, false, false, false);
	//============================================================= Render Interface
	Interface->Render(Shaded, false, false, false);
	//============================================================= Render Text
	Text->Render(Shaded, false, false, false);

	//==================================================== Swap Front/Back Buffer
	Engine::Screen()->SwapBuffers();
}

void PlayState::UpdateInputs(GLfloat DeltaTime)
{
	//============================================================= State Change
	if (Engine::Screen()->Key(GLFW_KEY_KP_ADD))
	{
		Engine::SwitchGamestate(new PlayState, true);
	}
	if (Engine::Screen()->Key(GLFW_KEY_KP_SUBTRACT))
	{
		Engine::SwitchGamestate(new PlayState, false);
	}
	if (Engine::Screen()->Key(GLFW_KEY_KP_ENTER))
	{
		Engine::ReturnGamestate();
	}
	//============================================================= Target Change
	if (Engine::Screen()->Key(GLFW_KEY_1))
	{
		Link(Light);
	}
	if (Engine::Screen()->Key(GLFW_KEY_2))
	{
		Link(Terrain);
	}
	if (Engine::Screen()->Key(GLFW_KEY_3))
	{
		Link(Water);
	}
	if (DemoTarget)
	{
		glm::vec3 newAmbient = DemoTarget->Ambient();
		glm::vec3 newDiffuse = DemoTarget->Diffuse();
		glm::vec3 newSpecular = DemoTarget->Specular();
		//============================================================= Modifiers Increment
		if (Engine::Screen()->Key(GLFW_KEY_Q))
		{
			if (Engine::Screen()->Key(GLFW_KEY_KP_7))
			{
				newAmbient.r += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_8))
			{
				newAmbient.g += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_9))
			{
				newAmbient.b += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_4))
			{
				newDiffuse.r += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_5))
			{
				newDiffuse.g += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_6))
			{
				newDiffuse.b += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_1))
			{
				newSpecular.r += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_2))
			{
				newSpecular.g += DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_3))
			{
				newSpecular.b += DeltaTime;
			}
		}
		//============================================================= Modifiers Decrement
		if (Engine::Screen()->Key(GLFW_KEY_E))
		{
			if (Engine::Screen()->Key(GLFW_KEY_KP_7))
			{
				newAmbient.r -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_8))
			{
				newAmbient.g -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_9))
			{
				newAmbient.b -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_4))
			{
				newDiffuse.r -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_5))
			{
				newDiffuse.g -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_6))
			{
				newDiffuse.b -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_1))
			{
				newSpecular.r -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_2))
			{
				newSpecular.g -= DeltaTime;
			}
			if (Engine::Screen()->Key(GLFW_KEY_KP_3))
			{
				newSpecular.b -= DeltaTime;
			}
		}
		if (Engine::Screen()->Key(GLFW_KEY_N) && !Engine::Screen()->Kill(GLFW_KEY_N))
		{
			Engine::Screen()->Kill(GLFW_KEY_N) = true;

			newAmbient = glm::vec3(1.0f) - DemoTarget->Ambient();
			newDiffuse = glm::vec3(1.0f) - DemoTarget->Diffuse();
			newSpecular = glm::vec3(1.0f) - DemoTarget->Specular();
		}
		DemoTarget->Ambient() = newAmbient;
		DemoTarget->Diffuse() = newDiffuse;
		DemoTarget->Specular() = newSpecular;
	}
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
	//============================================================= Render Skybox
	Skybox->Render(Shaded, false, false, false);
	//============================================================= Update Light
	Light->Render(false, false, false, Shaded);
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
	//============================================================= Render Skybox
	Skybox->Render(Shaded, false, false, false);
	//============================================================= Update Light
	Light->Render(false, false, false, Shaded);
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
	//============================================================= Terminate Skybox
	Skybox->Terminate();
	//============================================================= Terminate Light
	Light->Terminate();
	//============================================================= Terminate Terrain
	Terrain->Terminate();
	//============================================================= Terminate Water
	Water->Terminate();
	//============================================================= Terminate Player
	Player->Terminate();
	//============================================================= Terminate Interface
	Interface->Terminate();
	//============================================================= Terminate Text
	Text->Terminate();
}

PlayState::~PlayState()
{
	//============================================================= Destroy Camera
	delete Camera;
	//============================================================= Destroy Skybox
	delete Skybox;
	//============================================================= Destroy Light
	delete Light;
	//============================================================= Destroy Terrain
	delete Terrain;
	//============================================================= Destroy Water
	delete Water;
	//============================================================= Destroy Player
	delete Player;
	//============================================================= Destroy Interface
	delete Interface;
	//============================================================= Destroy Text
	delete Text;
}
