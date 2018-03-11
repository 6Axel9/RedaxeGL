#include "BView.h"
#include "Engine.h"
#include <gtc\matrix_transform.hpp>

BView::BView()
{
	//==================================================== Link Settings
	setup     = Engine::Settings();
	//==================================================== Link Shader
	locations = Engine::Shader()->UniformsID();
}

BView::~BView()
{
}

void BView::translate(glm::vec3 Target)
{
	//==================================================== Reflect View Matrix
	refPosition = glm::vec3(Target.x, -Target.y, Target.z);
	//==================================================== Translate Position
	position = Target;
}

void BView::rotate(glm::vec3 Target)
{
	//==================================================== Convert To Radiants
	GLfloat pitch   = glm::radians(Target.x);
	GLfloat yaw     = glm::radians(Target.y);
	GLfloat roll    = glm::radians(Target.z);
	GLfloat rad90	= glm::radians(90.0f);
	//==================================================== Rotate Direction
	refDirectionFront = glm::vec3(cos(-pitch) * sin(yaw), sin(-pitch), cos(-pitch) * cos(yaw));
	directionFront = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
	directionRight = glm::vec3(sin(yaw - rad90), 0, cos(yaw - rad90));
	refDirectionUp = glm::cross(directionRight, refDirectionFront);
	directionUp    = glm::cross(directionRight, directionFront);
}

void BView::transform()
{
	//==================================================== View Matrix
	view = glm::lookAt(position, position + directionFront, directionUp);
	//==================================================== Reflected View Matrix
	refView = glm::lookAt(refPosition, refPosition + refDirectionFront, refDirectionUp);
}

void BView::perspective()
{
	glEnable(GL_DEPTH_TEST);
	//==================================================== Perspective View
	proj = glm::perspective(glm::radians(60.0f), Engine::Screen()->Size().x / 
												 Engine::Screen()->Size().y, 0.1f, 1000.0f);
}

void BView::orthographic()
{
	glDisable(GL_DEPTH_TEST);
	//==================================================== Orthographic View
	proj = glm::ortho( Engine::Screen()->Size().x / 2,
					  -Engine::Screen()->Size().x / 2, -Engine::Screen()->Size().y / 2,
													    Engine::Screen()->Size().y / 2, 0.1f, 1000.0f);
	//==================================================== Orthographic Camera
	view = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
}

std::map<std::string, std::string> BView::setup;
std::map<std::string, GLuint> BView::locations;