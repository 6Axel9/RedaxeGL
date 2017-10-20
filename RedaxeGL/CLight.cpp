#include "CLight.h"

CLight::CLight()
{
}

void CLight::Enlighten(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Attenuation)
{
	//==================================================== Set Light Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	attenuation = Attenuation;
}

void CLight::Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed)
{
	//==================================================== Flip X
	Position.x = -Position.x;
	//==================================================== Initialize Position
	move = Position;
	//==================================================== Initialize Rotation
	angle = -Rotation;
	//==================================================== Initialize Scale
	size = Scale;
}

void CLight::Update(GLfloat DeltaTime)
{
	//==================================================== Update Tranformations
	translate(move);
	rotate(angle);
	scale(size);
	//==================================================== Update Transformations
	transform();
}

void CLight::Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit)
{
	//==================================================== Send Material
	glUniform3fv(locations["light.Ambient"], 1, &ambient.r);
	glUniform3fv(locations["light.Diffuse"], 1, &diffuse.r);
	glUniform3fv(locations["light.Specular"], 1, &specular.r);
	glUniform3fv(locations["light.Position"], 1, &position.x);
	glUniform3fv(locations["light.Direction"], 1, &direction.x);
	glUniform1f(locations["light.Attenuation"], attenuation);
}

void CLight::Terminate()
{
	//==================================================== Terminate
}


CLight::~CLight()
{
}
