#pragma once
#include "BModel.h"

class CInterface : public BModel
{
public:

	//==================================================== Create Text Editor
	CInterface(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Text
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale);
	//==================================================== Initialize Text Materials
	void Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess);
	//==================================================== Update Text
	void Update(GLfloat DeltaTime);
	//==================================================== Render Text
	void Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded);
	//==================================================== Terminate Text
	void Terminate();
	//==================================================== Destroy Text Editor
	~CInterface();

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

