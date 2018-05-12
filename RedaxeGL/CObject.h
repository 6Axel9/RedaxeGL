#pragma once
#include "BModel.h"

class CObject : public BModel
{
public:

	//==================================================== Create 3D Object
	CObject(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Object Transform
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale);
	//==================================================== Initialize Object Materials
	void Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess);
	//==================================================== Update Object
	void Update(GLfloat DeltaTime);
	//==================================================== Render Object
	void Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded);
	//==================================================== Terminate Object
	void Terminate();
	//==================================================== Destroy 3D Object
	virtual ~CObject();

	//==================================================== Handlers
	glm::vec3& Ambient()  { return ambient; }
	glm::vec3& Diffuse()  { return diffuse; }
	glm::vec3& Specular() { return specular; }
	GLfloat& Constant()   { return shininess; }

private:

	//==================================================== Buffer Data
	std::map<GLint, GLuint> mesh;
	std::map<GLint, GLuint> texture;
	std::map<GLint, GLuint> effect;
	std::map<GLint, GLuint> vnum;
	//==================================================== Properties
	GLfloat shininess;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 angle;
	glm::vec3 move;
	glm::vec3 size;
};