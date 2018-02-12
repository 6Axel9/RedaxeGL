#pragma once
#include "BModel.h"

class CLight : public BModel
{
public:

	//==================================================== Create Default
	CLight();
	//==================================================== Create 3D Object
	CLight(std::string MeshTag);
	//==================================================== Initialize Object
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed);
	//==================================================== Initialize Object Light
	void Enlighten(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Attenuation);
	//==================================================== Update Object
	virtual void Update(GLfloat DeltaTime);
	//==================================================== Render Object
	virtual void Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded);
	//==================================================== Terminate Object
	void Terminate();
	//==================================================== Destroy 3D Object
	virtual~CLight();

private:

	//==================================================== Buffer Data
	std::map<GLint, GLuint> mesh;
	std::map<GLint, GLuint> vnum;
	//==================================================== Properties
	GLfloat shininess;
	GLfloat attenuation;
	glm::vec3 material;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 angle;
	glm::vec3 move;
	glm::vec3 size;
};

