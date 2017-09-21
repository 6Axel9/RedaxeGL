#include "CObject.h"
#include "Engine.h"

CObject::CObject()
{
}

CObject::CObject(std::string MeshTag, std::string TextureTag, std::string EffectTag)
{
	//==================================================== Link Tag Data
	mesh	= Engine::Loader()->Models(MeshTag);
	texture = Engine::Loader()->Images(TextureTag);
	effect	= Engine::Loader()->Sounds(EffectTag);
	vnum	= Engine::Loader()->VNum(MeshTag);
}

void CObject::Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed)
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

void CObject::Update(GLfloat DeltaTime)
{
	//==================================================== Update Tranformations
	translate(move);
	rotate(angle);
	scale(size);
	//==================================================== Update Transformations
	transform();
}

void CObject::Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit)
{
	//==================================================== Send Booleans
	glUniform1i(locations["Textured"], Textured);
	glUniform1i(locations["Mapped"], Mapped);
	glUniform1i(locations["Lit"], Lit);
	//==================================================== Send Model Matrix
	glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &model[0][0]);
	//==================================================== Send Texture
	glUniform1i(locations["txtmap.Diffuse"], 0);

	//==================================================== Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//==================================================== Bind VAO
	glBindVertexArray(mesh[0]);
	//==================================================== Render
	glDrawElements(GL_TRIANGLES, vnum[0], GL_UNSIGNED_INT, 0);
	//==================================================== Unbind VAO
	glBindVertexArray(0);
	//==================================================== Unbind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CObject::Terminate()
{
	//==================================================== Terminate
}

CObject::~CObject()
{
}