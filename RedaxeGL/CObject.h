#pragma once
#include "BModel.h"

class CObject : public BModel
{
public:

	//==================================================== Create Default
	CObject();
	//==================================================== Create 3D Object
	CObject(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Object
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed);
	//==================================================== Update Object
	virtual void Update(GLfloat DeltaTime);
	//==================================================== Render Object
	virtual void Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit);
	//==================================================== Terminate Object
	void Terminate();
	//==================================================== Destroy 3D Object
	virtual~CObject();

private:

	//==================================================== Buffer Data
	std::map<GLint, GLuint> mesh;
	std::map<GLint, GLuint> texture;
	std::map<GLint, GLuint> effect;
	std::map<GLint, GLuint> vnum;
	//==================================================== Properties
	glm::vec3 angle;
	glm::vec3 move;
	glm::vec3 size;
};