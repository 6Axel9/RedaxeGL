#include "CSkyBox.h"
#include "Engine.h"

CSkyBox::CSkyBox()
{
}

CSkyBox::CSkyBox(std::string MeshTag, std::string TextureTag, std::string EffectTag)
{
	//==================================================== Link Tag Data
	mesh = Engine::Loader()->Models(MeshTag);
	texture = Engine::Loader()->Images(TextureTag);
	effect = Engine::Loader()->Sounds(EffectTag);
	vnum = Engine::Loader()->VNum(MeshTag);
}

void CSkyBox::Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess)
{
	//==================================================== Set Object Material Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	shininess = Shininess;
}

void CSkyBox::Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed)
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

void CSkyBox::Update(GLfloat DeltaTime)
{
	//==================================================== Update Tranformations
	translate(move);
	rotate(angle);
	scale(size);
	//==================================================== Update Transformations
	transform();
}

void CSkyBox::Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded)
{
	//==================================================== Send Booleans
	glUniform1i(locations["DiffuseMap"], Diffuse);
	glUniform1i(locations["SpecularMap"], Specular);
	glUniform1i(locations["NormalMap"], Normals);
	glUniform1i(locations["Shaded"], Shaded);
	//==================================================== Send Type
	glUniform1i(locations["TerrainShader"], false);
	glUniform1i(locations["WaterShader"], false);
	glUniform1i(locations["SkyboxShader"], true);
	//==================================================== Send Model Matrix
	glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &model[0][0]);
	//==================================================== Send Texture
	glUniform1i(locations["cubemap.Skybox"], 15);
	//==================================================== Send Material
	glUniform3fv(locations["material.Ambient"], 1, &ambient.r);
	glUniform3fv(locations["material.Diffuse"], 1, &diffuse.r);
	glUniform3fv(locations["material.Specular"], 1, &specular.r);
	glUniform1f(locations["material.Shininess"], shininess);

	//==================================================== Bind Texture
	for (GLuint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE15 + Map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture[Map]);
	}
	//==================================================== Disable Depth Mask
	//glDepthMask(GL_FALSE);
	//==================================================== Bind VAO
	glBindVertexArray(mesh[0]);
	//==================================================== Render
	glDrawElements(GL_TRIANGLES, vnum[0], GL_UNSIGNED_INT, 0);
	//==================================================== Unbind VAO
	glBindVertexArray(0);
	//==================================================== Enable Depth Mask
	//glDepthMask(GL_TRUE);
	//==================================================== Unbind Texture
	for (GLuint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE15 + Map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

void CSkyBox::Terminate()
{
	//==================================================== Terminate
}

CSkyBox::~CSkyBox()
{
}
