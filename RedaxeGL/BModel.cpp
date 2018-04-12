#include "BModel.h"
#include "Engine.h"
#include <gtc\matrix_transform.hpp>

BModel::BModel()
{
	//==================================================== Link Settings
	setup     = Engine::Settings();
	//==================================================== Link Shader
	locations = Engine::Shader()->UniformsID();
}

BModel::~BModel()
{
}

void BModel::translate(glm::vec3 Target)
{
	glm::vec3 newPosition = Target;
	//==================================================== Translate Matrix
	translation = glm::translate(glm::mat4(1), newPosition);
	//==================================================== Store New Position
	position = newPosition;
}

void BModel::rotate(glm::vec3 Target)
{
	//==================================================== Convert To Radiants
	GLfloat pitch   = glm::radians(Target.x);
	GLfloat yaw     = glm::radians(Target.y);
	GLfloat roll    = glm::radians(Target.z);
	GLfloat rad90   = glm::radians(90.0f);
	//==================================================== Rotate Matrix
	rotationX = glm::rotate(glm::mat4(1), pitch, glm::vec3(1, 0, 0));
	rotationY = glm::rotate(glm::mat4(1), yaw,	 glm::vec3(0, 1, 0));
	rotationZ = glm::rotate(glm::mat4(1), roll,  glm::vec3(0, 0, 1));
	//==================================================== Store Direction
	directionFront = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
	directionRight = glm::vec3(sin(yaw - rad90), 0, cos(yaw - rad90));
	directionUp	   = glm::cross(directionRight, directionFront);
}

void BModel::scale(glm::vec3 Target)
{
	//==================================================== Scale Matrix
	scaling = glm::scale(glm::mat4(1), Target);
}

void BModel::transform(glm::mat4& Target)
{
	//==================================================== Transform Matrix
	Target = translation * rotationZ * rotationY * rotationX * scaling;
}

void BModel::transform()
{
	//==================================================== Transform Matrix
	model = translation * rotationZ * rotationY * rotationX * scaling;
}

std::map<std::string, std::string> BModel::setup;
std::map<std::string, GLuint> BModel::locations;