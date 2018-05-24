#pragma once
#include "BModel.h"

class CText : public BModel
{
public:

	//==================================================== Create 2D Text
	CText(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Text Transform
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, std::string Text);
	//==================================================== Initialize Text Materials
	void Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess);
	//==================================================== Update Text
	void Update(GLfloat DeltaTime);
	//==================================================== Render Text
	void Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded);
	//==================================================== Terminate Text
	void Terminate();
	//==================================================== Destroy 2D Text
	virtual ~CText();

	//==================================================== Handlers
	glm::vec3& Ambient() { return ambient; }
	glm::vec3& Diffuse() { return diffuse; }
	glm::vec3& Specular() { return specular; }
	GLfloat& Constant() { return shininess; }
	std::string Tag() { return tag; }

private:

	//==================================================== Buffer Data
	std::map<GLint, GLuint> mesh;
	std::map<GLint, GLuint> texture;
	std::map<GLint, GLuint> effect;
	std::map<GLint, GLuint> vnum;
	//==================================================== Properties
	std::map<std::string, std::vector<glm::mat4>> models;
	std::map<std::string, std::vector<glm::vec3>> offsets;
	std::map<std::string, std::vector<glm::vec3>> angles;
	std::map<std::string, std::vector<glm::vec3>> sizes;
	GLfloat shininess;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::string tag;
};