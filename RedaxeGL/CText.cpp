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

void CText::Render(GLboolean Textured, GLboolean Mapped, GLboolean Lit)
{
	//==================================================== Send Booleans
	glUniform1i(locations["Textured"], Textured);
	glUniform1i(locations["Mapped"], Mapped);
	glUniform1i(locations["Lit"], Lit);
	//==================================================== Send Texture
	glUniform1i(locations["txtmap.Diffuse"], 0);

	//==================================================== Delete VAO
	for (auto const &Tag : offsets)
	{
		for (GLuint Char = 0; Char < Tag.first.size(); Char++)
		{
			GLint ascii = Tag.first[Char];
			//==================================================== Send Model Matrix
			glUniformMatrix4fv(locations["modelIn"], 1, GL_FALSE, &models[Tag.first][Char][0][0]);

			//==================================================== Bind Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			//==================================================== Bind VAO
			glBindVertexArray(mesh[ascii]);
			//==================================================== Render
			glDrawElements(GL_TRIANGLES, vnum[ascii], GL_UNSIGNED_INT, 0);
			//==================================================== Unbind VAO
			glBindVertexArray(0);
			//==================================================== Unbind Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
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