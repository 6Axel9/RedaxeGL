#include "Engine.h"
#include "PlayState.h"
#include "Tools.h"

Engine::Engine(std::string SetPath)
{
	readSetting(SetPath);
}

void Engine::Inititialize()
{
	//==================================================== Create Screen
	screen = new GLScreen(settings["Title"], 
		  StringToInteger(settings["Screen.width"]), 
		  StringToInteger(settings["Screen.height"]), 
		  StringToInteger(settings["Profile"]), 
		  StringToInteger(settings["Version X"]), 
		  StringToInteger(settings["Version x"]));
	//==================================================== Create Shader
	shader = new GLShader(settings["Shader.vertex"], 
						  settings["Shader.fragment"]);
	//==================================================== Import Assets
	loader = new GLLoader(settings["Import.models"],
						  settings["Import.images"],
						  settings["Import.sounds"]);
	//==================================================== Create Gamestate
	NewGamestate(new PlayState);
}

void Engine::SwitchGamestate(GameState* Next, bool Temporary)
{
	if (Temporary)
	{
		//==================================================== Queue To Deletion
		Gamestate.front()->Status() = false;
		//==================================================== Create Next Gamestate
		Gamestate.push_front(Next);
		//==================================================== Activate Gamestate
		Gamestate.front()->Status() = true;
		//============================================================= On Enter
		Gamestate.front()->OnEnter();
	}
	else
	{
		//==================================================== Queue To Deletion
		Gamestate.front()->Status() = false;
		//==================================================== Create Next Gamestate
		Gamestate.push_back(Next);
		//==================================================== Activate Gamestate
		Gamestate.back()->Status() = true;
		//============================================================= On Enter
		Gamestate.back()->OnEnter();
	}
}

void Engine::ReturnGamestate()
{
	//==================================================== Queue To Deletion
	Gamestate.front()->Status() = false;
	//==================================================== Queue To Restore
	Gamestate.back()->Status() = true;
}

void Engine::MainLoop()
{
	//==================================================== Create Timer
	auto startTime = std::chrono::high_resolution_clock::now();
	auto endTime = std::chrono::high_resolution_clock::now();
	//============================================================= Query Exit
	while (!Gamestate.empty() && !screen->Exit())
	{
		//============================================================= End Timer
		endTime = std::chrono::high_resolution_clock::now();
		deltaTime = DeltaTime(startTime, endTime);
		//============================================================= Start Timer
		startTime = std::chrono::high_resolution_clock::now();
		//==================================================== Update
		Gamestate.front()->Update(deltaTime);
		//==================================================== Render
		Gamestate.front()->Render(true);
		//============================================================= Delete Inactive Queue
		while (!Gamestate.front()->Status()) { DeleteGamestate(); }
	}
}

void Engine::Terminate()
{
	//============================================================= Delete Inactive Queue
	while (!Gamestate.empty()) { DeleteGamestate(); }
	//==================================================== Destroy Screen
	delete loader;
	delete shader;
	delete screen;
}

Engine::~Engine()
{
}

void Engine::readSetting(std::string SetPath)
{
	std::string last;
	std::ifstream file(SetPath);
	//==================================================== Restore Source File
	if (!file)
	{
		std::ofstream source(SetPath);
		source
			<< "REDAXE OPENGL ENGINE\n"
			<< "=============================================\n\n"
			<< "============ Engine Settings ================\n"
			<< "Title		= RedaxeGL;\n"
			<< "Profile		= 0x00032001;\n"
			<< "Version X	= 3;\n"
			<< "Version x	= 3;\n\n"
			<< "============ Screen Settings ================\n"
			<< "Screen.width	= 800;\n"
			<< "Screen.height	= 600;\n"
			<< "============ Shader Settings ================\n"
			<< "Shader.vertex	= Shaders\\Vertex.glsl;\n"
			<< "Shader.fragment	= Shaders\\Fragment.glsl;\n"
			<< "============ Import Settings ================\n"
			<< "Import.models	= Models\\Loader.dat;\n"
			<< "Import.images	= Images\\Loader.dat;\n"
			<< "Import.sounds	= Sounds\\Loader.dat;\n"
			<< "=============================================\n\n"
			<< "=============================================\n"
			<< "END";
		source.close();
		file.open(SetPath);
	}
	//==================================================== Read Settings
	while (last != "END")
	{
		std::getline(file, last);
		ScriptDecript(last, settings);
	}
	file.close();
}

void Engine::NewGamestate(GameState* New)
{
	//==================================================== Create Next Gamestate
	Gamestate.push_front(New);
	//==================================================== Activate Gamestate
	Gamestate.front()->Status() = true;
	//============================================================= On Enter
	Gamestate.front()->OnEnter();
}

void Engine::DeleteGamestate()
{
	//============================================================= On Exit
	Gamestate.front()->OnExit();
	//============================================================= Destroy GameState
	delete Gamestate.front();
	//============================================================= Clear Deque Space
	Gamestate.pop_front();
}

std::map<std::string, std::string> Engine::settings;
std::deque<GameState*> Engine::Gamestate;
GLScreen* Engine::screen;
GLShader* Engine::shader;
GLLoader* Engine::loader;