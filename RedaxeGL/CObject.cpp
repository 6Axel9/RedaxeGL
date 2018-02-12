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

void CObject::Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess)
{
	//==================================================== Set Object Material Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	shininess = Shininess;
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

void CObject::Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded)
{
	//==================================================== Send Booleans
	glUniform1i(locations["DiffuseMap"], Diffuse);
	glUniform1i(locations["SpecularMap"], Specular);
	glUniform1i(locations["NormalMap"], Normals);
	glUniform1i(locations["Shaded"], Shaded);
	//==================================================== Send Type
	glUniform1i(locations["TerrainShader"], false);
	glUniform1i(locations["WaterShader"], false);
	glUniform1i(locations["ShadowShader"], false);
	//==================================================== Send Model Matrix
	glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &model[0][0]);
	//==================================================== Send Texture
	glUniform1i(locations["txtmap.Diffuse"], 0);
	glUniform1i(locations["txtmap.Specular"], 1);
	glUniform1i(locations["txtmap.Normals"], 2);
	//==================================================== Send Material
	glUniform3fv(locations["material.Ambient"], 1, &ambient.r);
	glUniform3fv(locations["material.Diffuse"], 1, &diffuse.r);
	glUniform3fv(locations["material.Specular"], 1, &specular.r);
	glUniform1f(locations["material.Shininess"], shininess);

	//==================================================== Bind Texture
	for (GLint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE0 + Map);
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
		glActiveTexture(GL_TEXTURE0 + Map);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void CObject::Terminate()
{
	//==================================================== Terminate
}

CObject::~CObject()
{
}