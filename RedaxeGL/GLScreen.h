#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <string>
#include <map>

class GLScreen
{
public:
	
	//==================================================== Initialize OpenGL Screen
	GLScreen(std::string Title, GLint Width, GLint Height, GLint Mode, GLint VersionX, GLint VersionXX);
	//==================================================== Enable Alpha Blend
	void EnableBlending();
	//==================================================== Enable Clip Distance
	void EnableClipping();
	//==================================================== Enable Face Culling
	void EnableFaceCulling();
	//==================================================== Disable Alpha Blend
	void DisableBlending();
	//==================================================== Disable Clip Distance
	void DisableClipping();
	//==================================================== Disable Face Culling
	void DisableFaceCulling();
	//==================================================== Clear Current Buffer
	void ClearScreen();
	//==================================================== Swap Front/Back Buffer
	void SwapBuffers();
	//==================================================== Terminate OpenGL Screen
	~GLScreen();
	
	//==================================================== Handlers
	static GLint Exit() { return glfwWindowShouldClose(window); }
	static GLint Key(GLint KeyCode) { return key[KeyCode]; }
	static glm::vec4& Mouse() { return mouse; }
	static glm::vec2& Size() { return size; }

private:

	//==================================================== Create Window/OpenGL Context
	void createWindow(std::string Title, GLint Width, GLint Height, GLint Mode, GLint VersionX, GLint VersionXX);
	//==================================================== Assign Window Callback Functions
	void assignCallbacks();
	//==================================================== Load Functions
	void loadExtensions();
	//==================================================== On Resize
	static void onWindowResize(GLFWwindow* Window, GLint Width, GLint Height);
	//==================================================== On Mouse Key Handle
	static void onMouseKey(GLFWwindow* Window, GLint Key, GLint Action, GLint Special);
	//==================================================== On Mouse Movement Handle
	static void onMouseMove(GLFWwindow* Window, GLdouble MouseX, GLdouble MouseY);
	//==================================================== On Key Handle
	static void onKeyboard(GLFWwindow* Window, GLint Key, GLint ScanCode, GLint Action, GLint Special);

	//==================================================== Properties
	static std::map<GLint,GLint> key;
	static GLFWwindow* window;
	static glm::vec4 mouse;
	static glm::vec2 size;
};