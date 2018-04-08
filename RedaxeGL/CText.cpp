#include "CText.h"
#include "Engine.h"

CText::CText(std::string MeshTag, std::string TextureTag, std::string EffectTag)
{
	//==================================================== Link Tag Data
	mesh = Engine::Loader()->Models(MeshTag);
	texture = Engine::Loader()->Images(TextureTag);
	effect = Engine::Loader()->Sounds(EffectTag);
	vnum = Engine::Loader()->VNum(MeshTag);
}

void CText::Initialize(glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, std::string Text)
{
	Position.x -= (Text.size() - 1) * Scale.x / 4.0f;
	//==================================================== Initialize Text
	for (GLuint Space = 0; Space < Text.size(); Space++)
	{
		models[Text].push_back(glm::mat4(1));
		offsets[Text].push_back(Position);
		angles[Text].push_back(Rotation);
		sizes[Text].push_back(Scale);
		//==================================================== Next Space
		Position.x += Scale.x / 2;
	}
}

void CText::Materialize(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, GLfloat Shininess)
{
	//==================================================== Set Object Material Data
	ambient = Ambient;
	diffuse = Diffuse;
	specular = Specular;
	shininess = Shininess;
}

void CText::Update(GLfloat DeltaTime)
{
	for (auto const &Tag : offsets)
	{
		for (GLuint Char = 0; Char < Tag.first.size(); Char++)
		{
			GLint ascii = Tag.first[Char];
			//==================================================== Update Tranformations
			translate(offsets[Tag.first][Char]);
			rotate(angles[Tag.first][Char]);
			scale(sizes[Tag.first][Char]);
			//==================================================== Update Transformations
			transform(models[Tag.first][Char]);
		}
	}
}

void CText::Render(GLboolean Diffuse, GLboolean Specular, GLboolean Normals, GLboolean Shaded)
{
	//==================================================== Send Booleans
	glUniform1i(locations["DiffuseMap"], Diffuse);
	glUniform1i(locations["SpecularMap"], Specular);
	glUniform1i(locations["NormalMap"], Normals);
	glUniform1i(locations["Shaded"], Shaded);
	//==================================================== Send Type
	glUniform1i(locations["TerrainShader"], false);
	glUniform1i(locations["WaterShader"], false);
	glUniform1i(locations["SkyboxShader"], false);
	//==================================================== Send Texture
	glUniform1i(locations["txtmap.Diffuse"], 0);
	glUniform1i(locations["txtmap.Specular"], 1);
	glUniform1i(locations["txtmap.Normals"], 2);
	//==================================================== Send Material
	glUniform3fv(locations["material.Ambient"], 1, &ambient.r);
	glUniform3fv(locations["material.Diffuse"], 1, &diffuse.r);
	glUniform3fv(locations["material.Specular"], 1, &specular.r);
	glUniform1f(locations["material.Shininess"], shininess);

	//==================================================== Delete VAO
	for (auto const &Tag : offsets)
	{
		for (GLuint Char = 0; Char < Tag.first.size(); Char++)
		{
			GLint ascii = Tag.first[Char];
			//==================================================== Send Model Matrix
			glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &models[Tag.first][Char][0][0]);

			//==================================================== Bind Texture
			for (GLuint Map = 0; Map < texture.size(); Map++)
			{
				glActiveTexture(GL_TEXTURE0 + Map);
				glBindTexture(GL_TEXTURE_2D, texture[Map]);
			}
			//==================================================== Bind VAO
			glBindVertexArray(mesh[ascii]);
			//==================================================== Render
			glDrawElements(GL_TRIANGLES, vnum[ascii], GL_UNSIGNED_INT, 0);
			//==================================================== Unbind VAO
			glBindVertexArray(0);
			//==================================================== Unbind Texture
			for (GLuint Map = 0; Map < texture.size(); Map++)
			{
				glActiveTexture(GL_TEXTURE0 + Map);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
}

void CText::Terminate()
{
	//==================================================== Terminate
}

CText::~CText()
{
}