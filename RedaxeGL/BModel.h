#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include <map>

class BModel
{
public:
	
	//==================================================== Create Base Class
	BModel();
	//==================================================== Destroy Base Class
	virtual~BModel();

	//==================================================== Handlers
	virtual glm::vec3& Ambient() = 0;
	virtual glm::vec3& Diffuse() = 0;
	virtual glm::vec3& Specular() = 0;
	virtual GLfloat& Constant() = 0;

protected:

	//==================================================== Translate
	void translate(glm::vec3 Target);
	//==================================================== Rotate
	void rotate(glm::vec3 Target);
	//==================================================== Scale
	void scale(glm::vec3 Target);
	//==================================================== Transform
	void transform(glm::mat4& Target);
	//==================================================== Transform
	void transform();
	//==================================================== Properties
	static std::map<std::string, std::string> setup;
	static std::map<std::string, GLuint> locations;
	glm::vec3 directionFront;
	glm::vec3 directionRight;
	glm::vec3 directionUp;
	glm::mat4 translation;
	glm::vec3 position;
	glm::mat4 rotationZ;
	glm::mat4 rotationY;
	glm::mat4 rotationX;
	glm::mat4 scaling;
	glm::mat4 model;
};