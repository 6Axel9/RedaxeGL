#include "CWater.h"
#include "Engine.h"

CWater::CWater()
{
}

CWater::CWater(std::string MeshTag, std::string TextureTag, std::string EffectTag)
{
	//==================================================== Link Tag Data
	mesh = Engine::Loader()->Models(MeshTag);
	frame = Engine::Loader()->FrameBuffers(TextureTag);
	depth = Engine::Loader()->DepthBuffers(TextureTag);
	texture = Engine::Loader()->Images(TextureTag);
	effect = Engine::Loader()->Sounds(EffectTag);
	vnum = Engine::Loader()->VNum(MeshTag);
}

void CWater::Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess)
{
	//==================================================== Set Object Material Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	shininess = Shininess;
}

void CWater::Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GLfloat Speed)
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

void CWater::Update(GLfloat DeltaTime)
{
	//==================================================== Update Tranformations
	translate(move);
	rotate(angle);
	scale(size);
	//==================================================== Update Transformations
	transform();
}

void CWater::Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded)
{
	//==================================================== Send Booleans
	glUniform1i(locations["DiffuseMap"], Diffuse);
	glUniform1i(locations["SpecularMap"], Specular);
	glUniform1i(locations["NormalMap"], Normals);
	glUniform1i(locations["Shaded"], Shaded);
	//==================================================== Send Type
	glUniform1i(locations["TerrainShader"], false);
	glUniform1i(locations["WaterShader"], true);
	glUniform1i(locations["ShadowShader"], false);
	//==================================================== Send Model Matrix
	glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &model[0][0]);
	//==================================================== Send Texture
	glUniform1i(locations["water.Reflection"], 10);
	glUniform1i(locations["water.Refraction"], 11);
	glUniform1i(locations["water.Distortion"], 12);
	glUniform1i(locations["water.Normals"],	   13);
	glUniform1i(locations["water.Depth"],	   14);
	//==================================================== Send Material
	glUniform3fv(locations["material.Ambient"],  1, &ambient.r);
	glUniform3fv(locations["material.Diffuse"],  1, &diffuse.r);
	glUniform3fv(locations["material.Specular"], 1, &specular.r);
	glUniform1f(locations["material.Shininess"], shininess);

	//==================================================== Bind Texture
	for (GLuint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE10 + Map);
		glBindTexture(GL_TEXTURE_2D, texture[Map]);
	}	
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, depth[1]);
	//==================================================== Bind VAO
	glBindVertexArray(mesh[0]);
	//==================================================== Render
	glDrawElements(GL_TRIANGLES, vnum[0], GL_UNSIGNED_INT, 0);
	//==================================================== Unbind VAO
	glBindVertexArray(0);
	//==================================================== Unbind Texture
	for (GLuint Map = 0; Map < texture.size(); Map++)
	{
		glActiveTexture(GL_TEXTURE10 + Map);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CWater::Terminate()
{
	//==================================================== Terminate
}

CWater::~CWater()
{
}

GLuint CWater::Reflection()
{
	GLfloat Offset = 0.001f;
	//==================================================== Set Clip Plane
	clip = glm::vec4(0.0f, 1.0f, 0.0f, -position.y + Offset);
	//==================================================== Send Plane
	glUniform4fv(locations["clipPlane"], 1, &clip.x);
	//==================================================== Return Frame Buffer
	return frame[0];
}

GLuint CWater::Refraction()
{
	//==================================================== Set Clip Plane
	clip = glm::vec4(0.0f, -1.0f, 0.0f, -position.y);
	//==================================================== Send Plane
	glUniform4fv(locations["clipPlane"], 1, &clip.x);
	//==================================================== Return Frame Buffer
	return frame[1];
}
