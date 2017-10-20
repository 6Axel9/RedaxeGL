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
	glm::mat4 translation;
	glm::vec3 position;
	glm::mat4 rotationZ;
	glm::mat4 rotationY;
	glm::mat4 rotationX;
	glm::vec3 direction;
	glm::mat4 scaling;
	glm::mat4 model;
};