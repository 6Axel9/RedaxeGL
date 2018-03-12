#pragma once
#include "BView.h"
#include "MyExtension.h"

class CCamera : public BView
{
public:

	//==================================================== Create 3D Camera
	CCamera();
	//==================================================== Initialize Camera
	void Initialize(glm::vec3 Position, glm::vec3 Rotation, GLfloat Speed);
	//==================================================== Update Camera
	virtual void Update(GLfloat DeltaTime);
	//==================================================== Render Camera
	virtual void Render(GLboolean Perspective, GLboolean Reflective);
	//==================================================== Terminate Camera
	void Terminate();
	//==================================================== Destroy 3D Camera
	virtual~CCamera();

	//==================================================== Controller Input
	MyExtension* microbit;

private:

	//==================================================== Properties
	glm::vec3 angle;
	glm::vec3 move;
	GLfloat speed;
};