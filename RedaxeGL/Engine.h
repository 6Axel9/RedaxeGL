#pragma once
#include "GLScreen.h"
#include "GLShader.h"
#include "GLLoader.h"
#include "GameState.h"
#include <deque>

class Engine
{
public:

	//==================================================== Read Engine Setting
	Engine(std::string SetPath);
	//==================================================== Initialize Demo
	void Inititialize();
	//==================================================== New Game State
	static void NewGamestate(GameState* NextState);
	//==================================================== Switch Game State
	static void SwitchGamestate(GameState* NextState, bool Temporary);
	//==================================================== Return Game State
	static void ReturnGamestate();
	//==================================================== Delete Gamestate
	static void DeleteGamestate();
	//==================================================== Loop Demo
	void MainLoop();
	//==================================================== Terminate Demo
	void Terminate();

	virtual ~Engine();

	//==================================================== Handlers
	static std::map<std::string, std::string>& Settings() { return settings; }
	static GLScreen*& Screen() { return screen; }
	static GLShader*& Shader() { return shader; }
	static GLLoader*& Loader() { return loader; }

private:

	//==================================================== Read Initialization Settings
	void readSetting(std::string SetPath);
	//==================================================== Properties
	static std::map<std::string, std::string> settings;
	static std::deque<GameState*> Gamestate;
	static GLScreen* screen;
	static GLShader* shader;
	static GLLoader* loader;
	GLfloat deltaTime;
};