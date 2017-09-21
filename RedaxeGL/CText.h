#pragma once
#include "BModel.h"

class CText : public BModel
{
public:

	//==================================================== Create Text Editor
	CText(std::string MeshTag, std::string TextureTag, std::string EffectTag);
	//==================================================== Initialize Text
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, std::string Text);
	//==================================================== Update Text
	void Update(GLfloat DeltaTime);
	//==================================================== Render Text
	void Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit);
	//==================================================== Terminate Text
	void Terminate();
	//==================================================== Destroy Text Editor
	~CText();

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
};