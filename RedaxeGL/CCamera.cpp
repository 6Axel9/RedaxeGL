#include "CCamera.h"
#include "Engine.h"
#include "Tools.h"
#include <gtc\matrix_transform.hpp>

CCamera::CCamera()
{
	microbit = new MyExtension("serial_micro_values");
}

void CCamera::Initialize(glm::vec3 Position, glm::vec3 Rotation, GLfloat Speed)
{
	//==================================================== Flip X
	Position.x = -Position.x;
	//==================================================== Initialize Position
	move = Position;
	//==================================================== Initialize Rotation
	angle = -Rotation;
	//==================================================== Initialize Speed
	speed = Speed;
	//==================================================== Controller Init
	microbit->Connect();
}

void CCamera::Update(GLfloat DeltaTime)
{
	//==================================================== Update Controller Input
	//microbit->IntialiseVarTuple();
	//==================================================== Move Camera
	if (Engine::Screen()->Key(GLFW_KEY_W))
	{
		move += directionFront * DeltaTime;
	}
	if (Engine::Screen()->Key(GLFW_KEY_A))
	{
		move -= directionRight * DeltaTime;
	}
	if (Engine::Screen()->Key(GLFW_KEY_S))
	{
		move -= directionFront * DeltaTime;
	}
	if (Engine::Screen()->Key(GLFW_KEY_D))
	{
		move += directionRight * DeltaTime;
	}
	//==================================================== Rotate Camera
	angle += glm::vec3(-Engine::Screen()->Mouse().w - microbit->GetPitch() / 1000.0f,
					   -Engine::Screen()->Mouse().z + microbit->GetRoll() / 1000.0f, 0.0f) * DeltaTime * speed;
	//==================================================== Limit Movements
	if (angle.x >  70) { angle.x =  70; }
	if (angle.x < -70) { angle.x = -70; }
	//==================================================== Update Tranformations
	translate(move);
	rotate(angle);
	//==================================================== Update Transformations
	transform();
}


void CCamera::Render(GLboolean Perspective)
{
	//==================================================== Perspective Mode
	if (Perspective) { perspective(); }
	//==================================================== Orthographic Mode
	else			 { orthographic(); }

	//==================================================== Send Projection Matrix
	glUniformMatrix4fv(locations["projIn"], 1, GL_FALSE, &proj[0][0]);
	//==================================================== Send View Mtrix
	glUniformMatrix4fv(locations["viewIn"], 1, GL_FALSE, &view[0][0]);
	//==================================================== Send Camera Position
	glUniform3fv(locations["camera.Position"], 1, &position.x);
}

void CCamera::Terminate()
{
	//==================================================== Terminate
}

CCamera::~CCamera()
{
}
