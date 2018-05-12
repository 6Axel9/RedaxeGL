#pragma once
#include "BModel.h"

class CLight : public BModel
{
public:

	//==================================================== Create 3D Light
	CLight(std::string MeshTag);
	//==================================================== Initialize Light Transform
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale);
	//==================================================== Initialize Light Materials
	void Enlighten(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Attenuation);
	//==================================================== Update Light
	void Update(GLfloat DeltaTime);
	//==================================================== Render Light
	void Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded);
	//==================================================== Terminate Light
	void Terminate();
	//==================================================== Destroy 3D Light
	virtual ~CLight();

	//==================================================== Handlers
	glm::vec3& Ambient()  { return ambient; }
	glm::vec3& Diffuse()  { return diffuse; }
	glm::vec3& Specular() { return specular; }
	GLfloat& Constant()   { return attenuation; }

private:

	//==================================================== Buffer Data
	std::map<GLint, GLuint> mesh;
	std::map<GLint, GLuint> vnum;
	//==================================================== Properties
	GLfloat attenuation;
	glm::vec3 material;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 angle;
	glm::vec3 move;
	glm::vec3 size;
};

