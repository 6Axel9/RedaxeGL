#pragma once
#include "BModel.h"

class CInterface : public BModel
{
public:

	//==================================================== Create Text Editor
	CInterface(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Text
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale);
	//==================================================== Update Text
	void Update(GLfloat DeltaTime);
	//==================================================== Render Text
	void Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit);
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
	glm::vec3 angle;
	glm::vec3 move;
	glm::vec3 size;
};

