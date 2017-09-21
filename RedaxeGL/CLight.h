#pragma once
#include "BModel.h"

class CLight : public BModel
{
public:

	//==================================================== Create Default
	CLight();
	//==================================================== Create 3D Object
	CLight(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular);
	//==================================================== Initialize Object
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, GLfloat Attenuation);
	//==================================================== Update Object
	virtual void Update(GLfloat DeltaTime);
	//==================================================== Render Object
	virtual void Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit);
	//==================================================== Terminate Object
	void Terminate();
	//==================================================== Destroy 3D Object
	virtual~CLight();
};

