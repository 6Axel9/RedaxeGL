#pragma once
#include "BModel.h"

class CWater : public BModel
{
public:

	//==================================================== Create 3D Water
	CWater(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Water Transform
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Size);
	//==================================================== Initialize Water Materials
	void Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess);
	//==================================================== Update Water
	void Update(GLfloat DeltaTime);
	//==================================================== Render Water
	void Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded);
	//==================================================== Terminate Water
	void Terminate();
	//==================================================== Destroy 3D Water
	virtual ~CWater();

	//==================================================== Handlers
	glm::vec3& Ambient() { return ambient; }
	glm::vec3& Diffuse() { return diffuse; }
	glm::vec3& Specular() { return specular; }
	GLfloat& Constant() { return shininess; }
	//==================================================== Reflection
	GLuint Reflection();
	//==================================================== Refraction
	GLuint Refraction();
	
private:

	std::vector<glm::mat4> models;
	std::vector<glm::vec3> offsets;
	std::vector<glm::vec3> angles;
	std::vector<glm::vec3> sizes;
	//==================================================== Buffer Data
	std::map<GLint, GLuint> mesh;
	std::map<GLint, GLuint> frame;
	std::map<GLint, GLuint> depth;
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
	glm::vec4 clip;
};

