#include "GLScreen.h"
#include "Tools.h"

GLScreen::GLScreen(std::string Title, GLint Width, GLint Height, GLint Mode, GLint VersionX, GLint VersionXX)
{
	//==================================================== Create Window
	createWindow(Title, Width, Height, Mode, VersionX, VersionXX);
	//==================================================== Assign Callbacks
	assignCallbacks();
	//==================================================== Load OpenGL Extensions
	loadExtensions();
}

void GLScreen::EnableBlending()
{
	glEnable(GL_BLEND);
	//==================================================== Enable Alpha Channel Blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLScreen::EnableClipping()
{
	//==================================================== Enable Clipping Planes
	glEnable(GL_CLIP_DISTANCE0);
}

void GLScreen::EnableFaceCulling()
{
	//==================================================== Enable Face Culling
	glEnable(GL_CULL_FACE);
}

void GLScreen::DisableBlending()
{
	//==================================================== Disable Alpha Channel Blend
	glEnable(GL_BLEND);
}

void GLScreen::DisableClipping()
{
	//==================================================== Disable Clipping Planes
	glDisable(GL_CLIP_DISTANCE0);
}

void GLScreen::DisableFaceCulling()
{
	//==================================================== Disable Face Culling
	glDisable(GL_CULL_FACE);
}

void GLScreen::ClearScreen()
{
	//==================================================== Clear Screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLScreen::SwapBuffers()
{
	//==================================================== Reset Mouse Speed
	mouse = glm::vec4(mouse.x, mouse.y, 0, 0);
	//==================================================== Swap Frame Buffer
	glfwSwapBuffers(window);
	//==================================================== Update Events
	glfwPollEvents();
}

GLScreen::~GLScreen()
{
	//==================================================== Destroy Window
	glfwDestroyWindow(window);
	//==================================================== Terminate GLFW
	glfwTerminate();
}

void GLScreen::createWindow(std::string Title, GLint Width, GLint Height, GLint Mode, GLint VersionX, GLint VersionXX)
{
	//==================================================== Initialize GLFW
	glfwInit();
	//==================================================== Store Window Size
	size = glm::vec2(Width, Height);
	//==================================================== Set OpenGL Mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, Mode);
	//==================================================== Set OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VersionX);
	//==================================================== Set OpenGL SubVersion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VersionXX);
	//==================================================== Create GLFW Window
	window = glfwCreateWindow(Width, Height, Title.c_str(), glfwGetPrimaryMonitor(), NULL);

	//==================================================== Validate
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		//==================================================== Terminate GLFW
		glfwTerminate();
	}

	//==================================================== Disable Cursore
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//==================================================== Set Mouse Initial Position
	glfwSetCursorPos(window, size.x / 2, size.y / 2);
	//==================================================== Initialize Mouse Data
	mouse = glm::vec4(size.x / 2, size.y / 2, 0, 0);
	//==================================================== Create OpenGL Context
	glfwMakeContextCurrent(window);
	//==================================================== Unlock Framerate
	glfwSwapInterval(0);
}

void GLScreen::assignCallbacks()
{
	//==================================================== Set Resize CallBack
	glfwSetFramebufferSizeCallback(window, onWindowResize);
	//==================================================== Set Mouse Key CallBack
	glfwSetMouseButtonCallback(window, onMouseKey);
	//==================================================== Set Mouse Move CallBack
	glfwSetCursorPosCallback(window, onMouseMove);
	//==================================================== Set Keyboard CallBack
	glfwSetKeyCallback(window, onKeyboard);
}

void GLScreen::loadExtensions()
{
	//==================================================== Load OpenGL Functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}    
}

void GLScreen::onWindowResize(GLFWwindow* window, GLint width, GLint height)
{
	//==================================================== Resize Window
	glViewport(0, 0, width, height);
	//==================================================== Store Window Size
	size = glm::vec2(width, height);
}

void GLScreen::onKeyboard(GLFWwindow* Window, GLint KeyCode, GLint ScanCode, GLint Action, GLint Special)
{
	//==================================================== Store Key Data
	key[KeyCode] = Action;
	//==================================================== Key Repeat Kill
	if (Action == 0) { kill[KeyCode] = false; }
	//==================================================== Query ESC
	if (key[GLFW_KEY_ESCAPE])
	{
		//==================================================== Close Window
		glfwSetWindowShouldClose(window, true);
	}
}

void GLScreen::onMouseKey(GLFWwindow* Window, GLint Key, GLint Action, GLint Special)
{
}

void GLScreen::onMouseMove(GLFWwindow* Window, GLdouble MouseX, GLdouble MouseY)
{
	//==================================================== Get Accelleration
	glm::vec2 offset = glm::vec2(MouseX - mouse.x, MouseY - mouse.y);
	//==================================================== Store Mouse Data
	mouse = glm::vec4(mouse.x + offset.x, mouse.y + offset.y, offset.x, offset.y);
}

std::map<GLint, GLboolean> GLScreen::kill;
std::map<GLint, GLint> GLScreen::key;
GLFWwindow* GLScreen::window;
glm::vec4 GLScreen::mouse;
glm::vec2 GLScreen::size;