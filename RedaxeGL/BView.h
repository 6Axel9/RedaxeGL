#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include <string>
#include <map>

class BView
{
public:

	//==================================================== Create Base Class
	BView();
	//==================================================== Destry Base Class
	virtual~BView();

protected:

	//==================================================== Translate
	void translate(glm::vec3 Target);
	//==================================================== Rotate
	void rotate(glm::vec3 Target);
	//==================================================== Transform
	void transform();
	//==================================================== Orthographic
	void perspective();
	//==================================================== Perspective
	void orthographic();
	//==================================================== Properties
	static std::map<std::string, std::string> setup;
	static std::map<std::string, GLuint> locations;
	glm::vec3 directionFront;
	glm::vec3 directionRight;
	glm::vec3 directionUp;
	glm::vec3 position;
	glm::mat4 proj;
	glm::mat4 view;
};

