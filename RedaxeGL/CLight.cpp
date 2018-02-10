#include "CLight.h"
#include "Engine.h"

CLight::CLight()
{
}

CLight::CLight(std::string MeshTag)
{
	//==================================================== Link Tag Data
	mesh = Engine::Loader()->Models(MeshTag);
	vnum = Engine::Loader()->VNum(MeshTag);
}

void CLight::Enlighten(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Attenuation)
{
	//==================================================== Set Light Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	attenuation = Attenuation;
	material = diffuse * 10.0f;
	shininess = 1.0f;
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
	//==================================================== Send Booleans
	glUniform1i(locations["Textured"], Textured);
	glUniform1i(locations["MultiText"], false);
	glUniform1i(locations["Mapped"], Mapped);
	glUniform1i(locations["Lit"], Lit);
	//==================================================== Send Model Matrix
	glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &model[0][0]);
	//==================================================== Send Light Material
	glUniform3fv(locations["light.Ambient"], 1, &ambient.r);
	glUniform3fv(locations["light.Diffuse"], 1, &diffuse.r);
	glUniform3fv(locations["light.Specular"], 1, &specular.r);
	glUniform3fv(locations["light.Position"], 1, &position.x);
	glUniform3fv(locations["light.Direction"], 1, &direction.x);
	glUniform1f(locations["light.Attenuation"], attenuation);
	//==================================================== Send Object Material
	glUniform3fv(locations["material.Ambient"], 1, &material.r);
	glUniform3fv(locations["material.Diffuse"], 1, &material.r);
	glUniform3fv(locations["material.Specular"], 1, &material.r);
	glUniform1f(locations["material.Shininess"], material.r);

	//==================================================== Set Point Size
	glPointSize(25.0f);
	//==================================================== Bind VAO
	glBindVertexArray(mesh[0]);
	//==================================================== Render
	glDrawElements(GL_POINTS, vnum[0], GL_UNSIGNED_INT, 0);
	//==================================================== Unbind VAO
	glBindVertexArray(0);
}

void CLight::Terminate()
{
	//==================================================== Terminate
}


CLight::~CLight()
{
}
