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
	//============================================================= Create Interface
	Ambient.first = new CInterface("Box", "Interface", "None");
	Ambient.second = new CText("Font", "Font", "None");
	Diffuse.first = new CInterface("Box", "Interface", "None");
	Diffuse.second = new CText("Font", "Font", "None");
	Specular.first = new CInterface("Box", "Interface", "None");
	Specular.second = new CText("Font", "Font", "None");
	//============================================================= Create Buttons
	Red.first = new CInterface("Box", "Interface", "None");
	Red.second = new CText("Font", "Font", "None");
	Green.first = new CInterface("Box", "Interface", "None");
	Green.second = new CText("Font", "Font", "None");
	Blue.first = new CInterface("Box", "Interface", "None");
	Blue.second = new CText("Font", "Font", "None");

	//============================================================= Link Target Objects
	TargetObjects.push_back(Light);
	TargetObjects.push_back(Terrain);
	TargetObjects.push_back(Water);
	//============================================================= Link Target Buttons
	TargetButtons.push_back(Red);
	TargetButtons.push_back(Green);
	TargetButtons.push_back(Blue);
	//============================================================= Link Target Properties
	TargetProperties.push_back(Ambient);
	TargetProperties.push_back(Diffuse);
	TargetProperties.push_back(Specular);
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

	//============================================================= Initialize Buttons
	Red.first->Initialize(glm::vec3(275.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(35.0f, 35.0f, 1.0f));
	Red.second->Initialize(glm::vec3(275.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(25.0f), "R");
	Green.first->Initialize(glm::vec3(300.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(35.0f, 35.0f, 1.0f));
	Green.second->Initialize(glm::vec3(300.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(25.0f), "G");
	Blue.first->Initialize(glm::vec3(325, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(35.0f, 35.0f, 1.0f));
	Blue.second->Initialize(glm::vec3(325, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(25.0f), "B");

	Ambient.first->Initialize(glm::vec3(200.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(125.0f, 35.0f, 1.0f));
	Ambient.second->Initialize(glm::vec3(200.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Ambient");
	Diffuse.first->Initialize(glm::vec3(200.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(125.0f, 35.0f, 1.0f));
	Diffuse.second->Initialize(glm::vec3(200.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Diffuse");
	Specular.first->Initialize(glm::vec3(200.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(125.0f, 35.0f, 1.0f));
	Specular.second->Initialize(glm::vec3(200.0f, 250.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f), "Specular");
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
	//============================================================= Update Buttons
	TargetButtons.front().first->Update(DeltaTime);
	TargetButtons.front().second->Update(DeltaTime);

	TargetProperties.front().first->Update(DeltaTime);
	TargetProperties.front().second->Update(DeltaTime);
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
	Terrain->Render(true, Wireframed, Normalized, Shaded);
	//============================================================= Render Water
	Water->Render(true, Wireframed, Normalized, Shaded);

	//============================================================= Render Camera 2D
	Camera->Render(false, false);
	//============================================================= Render Buttons
	TargetButtons.front().first->Render(Shaded, false, false, false);
	TargetButtons.front().second->Render(Shaded, false, false, false);

	TargetProperties.front().first->Render(Shaded, false, false, false);
	TargetProperties.front().second->Render(Shaded, false, false, false);

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
	//============================================================= Target Button Change
	if (Engine::Screen()->Key(GLFW_KEY_LEFT) && !Engine::Screen()->Kill(GLFW_KEY_LEFT))
	{
		Engine::Screen()->Kill(GLFW_KEY_LEFT) = true;

		TargetButtons.push_front(TargetButtons.back());
		TargetButtons.pop_back();
	}
	if (Engine::Screen()->Key(GLFW_KEY_RIGHT) && !Engine::Screen()->Kill(GLFW_KEY_RIGHT))
	{
		Engine::Screen()->Kill(GLFW_KEY_RIGHT) = true;

		TargetButtons.push_back(TargetButtons.front());
		TargetButtons.pop_front();
	}
	//============================================================= Target Propert Change
	if (Engine::Screen()->Key(GLFW_KEY_DOWN) && !Engine::Screen()->Kill(GLFW_KEY_DOWN))
	{
		Engine::Screen()->Kill(GLFW_KEY_DOWN) = true;

		TargetProperties.push_front(TargetProperties.back());
		TargetProperties.pop_back();
	}
	if (Engine::Screen()->Key(GLFW_KEY_UP) && !Engine::Screen()->Kill(GLFW_KEY_UP))
	{
		Engine::Screen()->Kill(GLFW_KEY_UP) = true;

		TargetProperties.push_back(TargetProperties.front());
		TargetProperties.pop_front();
	}
	//============================================================= Target Object Change
	if (Engine::Screen()->Key(GLFW_KEY_TAB) && !Engine::Screen()->Kill(GLFW_KEY_TAB))
	{
		Engine::Screen()->Kill(GLFW_KEY_TAB) = true;

		TargetObjects.push_back(TargetObjects.front());
		TargetObjects.pop_front();
	}

	glm::vec3 newAmbient = TargetObjects.front()->Ambient();
	glm::vec3 newDiffuse = TargetObjects.front()->Diffuse();
	glm::vec3 newSpecular = TargetObjects.front()->Specular();
	//============================================================= Modifiers Increment
	if (Engine::Screen()->Key(GLFW_KEY_Q))
	{
		if (TargetButtons.front().second->Tag() == "R")
		{
			newAmbient.r += DeltaTime;
			newDiffuse.r += DeltaTime;
			newSpecular.r += DeltaTime;
		}
		if (TargetButtons.front().second->Tag() == "G")
		{
			newAmbient.g += DeltaTime;
			newDiffuse.g += DeltaTime;
			newSpecular.g += DeltaTime;
		}
		if (TargetButtons.front().second->Tag() == "B")
		{
			newAmbient.b += DeltaTime;
			newDiffuse.b += DeltaTime;
			newSpecular.b += DeltaTime;
		}
	}
	//============================================================= Modifiers Decrement
	if (Engine::Screen()->Key(GLFW_KEY_E))
	{
		if (TargetButtons.front().second->Tag() == "R")
		{
			newAmbient.r -= DeltaTime;
			newDiffuse.r -= DeltaTime;
			newSpecular.r -= DeltaTime;
		}
		if (TargetButtons.front().second->Tag() == "G")
		{
			newAmbient.g -= DeltaTime;
			newDiffuse.g -= DeltaTime;
			newSpecular.g -= DeltaTime;
		}
		if (TargetButtons.front().second->Tag() == "B")
		{
			newAmbient.b -= DeltaTime;
			newDiffuse.b -= DeltaTime;
			newSpecular.b -= DeltaTime;
		}
	}
	if (Engine::Screen()->Key(GLFW_KEY_N) && !Engine::Screen()->Kill(GLFW_KEY_N))
	{
		Engine::Screen()->Kill(GLFW_KEY_N) = true;

		newAmbient = glm::vec3(1.0f) - TargetObjects.front()->Ambient();
		newDiffuse = glm::vec3(1.0f) - TargetObjects.front()->Diffuse();
		newSpecular = glm::vec3(1.0f) - TargetObjects.front()->Specular();
	}
	if (Engine::Screen()->Key(GLFW_KEY_9) && !Engine::Screen()->Kill(GLFW_KEY_9))
	{
		Engine::Screen()->Kill(GLFW_KEY_9) = true;

		Normalized = !Normalized;
	}
	if (Engine::Screen()->Key(GLFW_KEY_8) && !Engine::Screen()->Kill(GLFW_KEY_8))
	{
		Engine::Screen()->Kill(GLFW_KEY_8) = true;

		Wireframed = !Wireframed;
	}
	if (Engine::Screen()->Key(GLFW_KEY_7) && !Engine::Screen()->Kill(GLFW_KEY_7))
	{
		Engine::Screen()->Kill(GLFW_KEY_7) = true;

		Normalized = true;
		Wireframed = false;
	}

	if (TargetProperties.front().second->Tag() == "Ambient")
	{
		TargetObjects.front()->Ambient() = newAmbient;
	}
	if (TargetProperties.front().second->Tag() == "Diffuse")
	{
		TargetObjects.front()->Diffuse() = newDiffuse;
	}
	if (TargetProperties.front().second->Tag() == "Specular")
	{
		TargetObjects.front()->Specular() = newSpecular;
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
	Terrain->Render(true, Wireframed, Normalized, Shaded);
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
	Terrain->Render(true, Wireframed, Normalized, Shaded);
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
	//============================================================= Terminate Buttons
	Ambient.first->Terminate();
	Ambient.second->Terminate();
	Diffuse.first->Terminate();
	Diffuse.second->Terminate();
	Specular.first->Terminate();
	Specular.second->Terminate();
	//============================================================= Create Buttons
	Red.first->Terminate();
	Red.second->Terminate();
	Green.first->Terminate();
	Green.second->Terminate();
	Blue.first->Terminate();
	Blue.second->Terminate();
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
	//============================================================= Destroy Buttons
	delete Ambient.first;
	delete Ambient.second;
	delete Diffuse.first;
	delete Diffuse.second;
	delete Specular.first;
	delete Specular.second;
	//============================================================= Create Buttons
	delete Red.first;
	delete Red.second;
	delete Green.first;
	delete Green.second;
	delete Blue.first;
	delete Blue.second;
}
