#include "CTerrain.h"
#include "Engine.h"

CTerrain::CTerrain()
{
}

CTerrain::CTerrain(std::string MeshTag, std::string TextureTag, std::string EffectTag)
{
	//==================================================== Link Tag Data
	mesh = Engine::Loader()->Models(MeshTag);
	texture = Engine::Loader()->Images(TextureTag);
	effect = Engine::Loader()->Sounds(EffectTag);
	vnum = Engine::Loader()->VNum(MeshTag);
}

void CTerrain::Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess)
{
	//==================================================== Set Object Material Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	shininess = Shininess;
}

void CTerrain::Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed)
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

void CTerrain::Update(GLfloat DeltaTime)
{
	//==================================================== Update Tranformations
	translate(move);
	rotate(angle);
	scale(size);
	//==================================================== Update Transformations
	transform();
}

void CTerrain::Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit)
{
	//==================================================== Send Booleans
	glUniform1i(locations["Textured"], Textured);
	glUniform1i(locations["MultiText"], true);
	glUniform1i(locations["Mapped"], Mapped);
	glUniform1i(locations["Lit"], Lit);
	//==================================================== Send Model Matrix
	glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &model[0][0]);
	//==================================================== Send Texture
	glUniform1i(locations["terrain.G0Diffuse"], 3);
	glUniform1i(locations["terrain.G0Specular"], 4);
	glUniform1i(locations["terrain.G0Normals"], 5);

	glUniform1i(locations["terrain.G1Diffuse"], 6);
	glUniform1i(locations["terrain.G1Specular"], 7);
	glUniform1i(locations["terrain.G1Normals"], 8);

	glUniform1i(locations["terrain.GNoise"], 9);
	//==================================================== Send Material
	glUniform3fv(locations["material.Ambient"], 1, &ambient.r);
	glUniform3fv(locations["material.Diffuse"], 1, &diffuse.r);
	glUniform3fv(locations["material.Specular"], 1, &specular.r);
	glUniform1f(locations["material.Shininess"], shininess);

	//==================================================== WireFrame On
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//==================================================== Bind Texture
	for (GLint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE3 + Map);
		glBindTexture(GL_TEXTURE_2D, texture[Map]);
	}
	//==================================================== Bind VAO
	glBindVertexArray(mesh[0]);
	//==================================================== Render
	glDrawElements(GL_TRIANGLES, vnum[0], GL_UNSIGNED_INT, 0);
	//==================================================== Unbind VAO
	glBindVertexArray(0);
	//==================================================== Unbind Texture
	for (GLint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE3 + Map);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//==================================================== WireFrame Off
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CTerrain::Terminate()
{
	//==================================================== Terminate
}

CTerrain::~CTerrain()
{
}
