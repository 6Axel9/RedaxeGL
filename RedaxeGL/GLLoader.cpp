#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GLLoader.h"
#include "Tools.h"
#include <postprocess.h>
#include <cimport.h>
#include <scene.h>
#include <time.h>


GLLoader::GLLoader(std::string ModPath, std::string ImgPath, std::string SndPath)
{
	//==================================================== Load Model Assets
	loadModels(ModPath);
	//==================================================== Load Image Assets
	loadImages(ImgPath);
}

GLLoader::~GLLoader()
{
	//==================================================== Delete VAO
	for(auto const &Tag : models) 
	{	for(auto const &Frame : Tag.second) 
		{
			glDeleteVertexArrays(1, &Frame.second);
		}
	}
	//==================================================== Delete TXT
	for (auto const &Tag : images)
	{
		for (auto const &Type : Tag.second)
		{
			glDeleteTextures(1, &Type.second);
		}
	}
	//==================================================== Delete VBO
	for(auto const &Tag : vbo) 
	{	for(auto const &Frame : Tag.second) 
		{
			glDeleteBuffers(1, &Frame.second);
		}
	}
	//==================================================== Delete EBO
	for(auto const &Tag : ebo) 
	{	for(auto const &Frame : Tag.second) 
		{
			glDeleteBuffers(1, &Frame.second);
		}
	}
}

void GLLoader::loadModels(std::string ModPath)
{
	std::string last;
	std::ifstream file(ModPath);
	std::vector<std::string> parts;
	//==================================================== No Database Found
	if (!file)
	{
		std::cout << "Failed to read model database" << std::endl;
	}
	else
	{
		//==================================================== Read Database Entries
		while (last != "END")
		{
			std::getline(file, last);
			ImportDecript(last, parts);
			//==================================================== Load Database Entry
			if (!parts.empty())
			{
				generate3DGeometry(parts);
			}
		}
		file.close();
	}
	//==================================================== Load Terrain
	generateTerrainGeometry();
	//==================================================== Load Water
	//generateWaterGeometry();
	//==================================================== Load Font
	generateFontGeometry();
	//==================================================== Load Box
	generate2DGeometry();
}

void GLLoader::loadImages(std::string ImgPath)
{
	std::string last;
	std::ifstream file(ImgPath);
	std::vector<std::string> parts;
	//==================================================== No Database Found
	if (!file)
	{
		std::cout << "Failed to read images database" << std::endl;
	}
	else
	{
		//==================================================== Read Database Entries
		while (last != "END")
		{
			std::getline(file, last);
			ImportDecript(last, parts);
			//==================================================== Load Database Entry
			if (!parts.empty())
			{
				generateTextureMap(parts);
			}
		}
		file.close();
	}
}

void GLLoader::loadSounds(std::string SndPath)
{
}

void GLLoader::generate3DGeometry(std::vector<std::string>& Parts)
{
	framesNum[Parts[0]] = StringToInteger(Parts[1]);

	for (GLint Frame = 0; Frame < framesNum[Parts[0]]; Frame++)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::string Path = Parts[3] + Parts[0] + NumberToString(Frame) + Parts[2];
		
		//==================================================== Load Obj File
		const aiScene* Scene = aiImportFile(Path.c_str(), aiProcess_ValidateDataStructure | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals |
														  aiProcess_CalcTangentSpace	  | aiProcess_FlipUVs	 );

		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			std::cout << "Failed to load " << Path << std::endl;
		}
		else if(Scene->HasMeshes())
		{
			for (GLuint Mesh = 0; Mesh < Scene->mNumMeshes; Mesh++)
			{
				if (Scene->mMeshes[Mesh]->HasNormals() && Scene->mMeshes[Mesh]->HasTextureCoords(0) && Scene->mMeshes[Mesh]->HasFaces() && Scene->mMeshes[Mesh]->HasTangentsAndBitangents())
				{
					for (GLuint Vertex = 0; Vertex < Scene->mMeshes[Mesh]->mNumVertices; Vertex++)
					{
						//==================================================== Vertices
						vertices.push_back({
							//==================================================== Positions
							{ glm::vec3(Scene->mMeshes[Mesh]->mVertices[Vertex].x,
										Scene->mMeshes[Mesh]->mVertices[Vertex].y,
										Scene->mMeshes[Mesh]->mVertices[Vertex].z) },
							//==================================================== Colors
							{ glm::vec3(1.0f, 1.0f, 1.0f) },
							//==================================================== Texture Coords
							{ glm::vec2(Scene->mMeshes[Mesh]->mTextureCoords[0][Vertex].x,
										Scene->mMeshes[Mesh]->mTextureCoords[0][Vertex].y) },
							//==================================================== Normals
							{ glm::vec3(Scene->mMeshes[Mesh]->mNormals[Vertex].x,
										Scene->mMeshes[Mesh]->mNormals[Vertex].y,
										Scene->mMeshes[Mesh]->mNormals[Vertex].z) },
							//==================================================== Tangents
							{ glm::vec3(Scene->mMeshes[Mesh]->mTangents[Vertex].x,
										Scene->mMeshes[Mesh]->mTangents[Vertex].y,
										Scene->mMeshes[Mesh]->mTangents[Vertex].z) },
							//==================================================== Bitangents
							{ glm::vec3(Scene->mMeshes[Mesh]->mBitangents[Vertex].x,
										Scene->mMeshes[Mesh]->mBitangents[Vertex].y,
										Scene->mMeshes[Mesh]->mBitangents[Vertex].z) }});
					}
					for (GLuint Face = 0; Face < Scene->mMeshes[Mesh]->mNumFaces; Face++)
					{
						//==================================================== Indices
						for (GLuint Index = 0; Index < Scene->mMeshes[Mesh]->mFaces[Face].mNumIndices; Index++)
						{
							indices.push_back(Scene->mMeshes[Mesh]->mFaces[Face].mIndices[Index]);
						}
					}
					//==================================================== Vertices Count
					vnum[Parts[0]][Frame] = indices.size();
				}
			}
			//==================================================== Release Obj File
			aiReleaseImport(Scene);

			//==================================================== Generate VAO
			glGenVertexArrays(1, &models[Parts[0]][Frame]);
			//==================================================== Generate VBO
			glGenBuffers(1, &vbo[Parts[0]][Frame]);
			//==================================================== Generate EBO
			glGenBuffers(1, &ebo[Parts[0]][Frame]);

			//==================================================== Bind VAO
			glBindVertexArray(models[Parts[0]][Frame]);
			//==================================================== Bind VBO
			glBindBuffer(GL_ARRAY_BUFFER, vbo[Parts[0]][Frame]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
			//==================================================== Bind EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[Parts[0]][Frame]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

			//==================================================== Attribute Location 0
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);
			//==================================================== Attribute Location 1
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			//==================================================== Attribute Location 2
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			//==================================================== Attribute Location 3
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
			glEnableVertexAttribArray(3);
			//==================================================== Attribute Location 4
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
			glEnableVertexAttribArray(4);
			//==================================================== Attribute Location 5
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(14 * sizeof(GLfloat)));
			glEnableVertexAttribArray(5);

			//==================================================== Unbind VAO
			glBindVertexArray(0);
			//==================================================== Unbind EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//==================================================== Unbind VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}

void GLLoader::generateTerrainGeometry()
{
	GLfloat amplitude = 32.0f;
	GLfloat roughness = 0.45f;
	GLint octaves = 4;
	GLint vcount = 64;
	GLint tsize = 64;

	GLuint seed = (unsigned int) time(NULL);	

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	for (GLint posX = 0; posX < vcount; posX++)
	{
		for (GLint posY = 0; posY < vcount; posY++)
		{
			GLfloat Height = 0;
			GLfloat HeightL = 0;
			GLfloat HeightR = 0;
			GLfloat HeightU = 0;
			GLfloat HeightD = 0;

			GLfloat Value = (float)pow(2, octaves - 1);
			//==================================================== Terrain & Vertices Location
			glm::vec2 SlotPos = glm::vec2(-tsize / 2, -tsize / 2);
			glm::vec2 VertPos = glm::vec2(posX / (vcount - 1.0f) * tsize, posY / (vcount - 1.0f) * tsize);
			//==================================================== Height Location
			for (GLint FreqN = 0; FreqN < octaves; FreqN++)
			{
				float freq = (float)(pow(2, FreqN) / Value);
				float ampl = (float)pow(roughness, FreqN) * amplitude;
				Height += SmoothInterpolNoise(posX * freq, posY * freq, vcount, seed) * ampl;
				HeightL += SmoothInterpolNoise((posX - 1) * freq, posY * freq, vcount, seed) * ampl;
				HeightR += SmoothInterpolNoise((posX + 1) * freq, posY * freq, vcount, seed) * ampl;
				HeightD += SmoothInterpolNoise(posX * freq, (posY - 1) * freq, vcount, seed) * ampl; 
				HeightU += SmoothInterpolNoise(posX * freq, (posY + 1) * freq, vcount, seed) * ampl;
			}
			//==================================================== Texture Location
			glm::vec2 TextPos = glm::vec2(posX / (vcount - 1.0f), posY / (vcount - 1.0f));
			//==================================================== Normal Location
			glm::vec3 Normal = glm::normalize(glm::vec3(HeightL - HeightR, 2.0f, HeightD - HeightU));
			//==================================================== Tangent & Bitangent Location
			glm::vec3 Tang1 = glm::cross(Normal, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::vec3 Tang2 = glm::cross(Normal, glm::vec3(0.0f, 1.0f, 0.0f));

			glm::vec3 Tangent;
			glm::vec3 Bitangent;

			if (glm::length(Tang1) > glm::length(Tang2))
			{
				Tangent = glm::normalize(Tang1);
				Bitangent = glm::cross(Tangent, Normal);
			}
			else
			{
				Tangent = glm::normalize(Tang2);
				Bitangent = glm::cross(Tangent, Normal);
			}
			//==================================================== Vertices
			vertices.push_back({ 
				//==================================================== Positions
				{ glm::vec3(VertPos.x + SlotPos.x, Height , VertPos.y + SlotPos.y) },
				//==================================================== Colors
				{ glm::vec3(1.0f, 1.0f, 1.0f) },
				//==================================================== Texture Coords
				{ glm::vec2(TextPos.x, TextPos.y) },
				//==================================================== Normals
				{ Normal },
				//==================================================== Tangents
				{ Tangent },
				//==================================================== Bitangents
				{ Bitangent }});
		}
	}
	for (GLint posX = 0; posX < vcount - 1; posX++)
	{
		for (GLint posY = 0; posY < vcount - 1; posY++)
		{
			//==================================================== Index Location
			GLint BL = ((posX + 1) * vcount) + posY;
			GLint TL = (posX * vcount) + posY;
			GLint TR = TL + 1;
			GLint BR = BL + 1;
			//==================================================== Indices
			indices.push_back(BL);
			indices.push_back(TL);
			indices.push_back(TR);
			indices.push_back(TR);
			indices.push_back(BR);
			indices.push_back(BL);
		}
	}
	//==================================================== Vertices Count
	vnum["Terrain"][0] = indices.size();

	//==================================================== Generate VAO
	glGenVertexArrays(1, &models["Terrain"][0]);
	//==================================================== Generate VBO
	glGenBuffers(1, &vbo["Terrain"][0]);
	//==================================================== Generate EBO
	glGenBuffers(1, &ebo["Terrain"][0]);

	//==================================================== Bind VAO
	glBindVertexArray(models["Terrain"][0]);
	//==================================================== Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo["Terrain"][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	//==================================================== Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo["Terrain"][0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

	//==================================================== Attribute Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//==================================================== Attribute Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//==================================================== Attribute Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//==================================================== Attribute Location 3
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	//==================================================== Attribute Location 4
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	//==================================================== Attribute Location 5
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(14 * sizeof(GLfloat)));
	glEnableVertexAttribArray(5);

	//==================================================== Unbind VAO
	glBindVertexArray(0);
	//==================================================== Unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//==================================================== Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLLoader::generateFontGeometry()
{
	framesNum["Font"] = 16 * 16;
	GLfloat csize = 1.0f / 16.0f;
	GLfloat offset = 0.5f;
	
	for (GLint Frame = 0; Frame < framesNum["Font"]; Frame++)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		//==================================================== Convert To Texture Coordinates
		glm::vec2 SlotCoord = glm::vec2(Frame % 16, Frame / 16);
		glm::vec2 TextCoord = glm::vec2(SlotCoord.x * csize + csize / 2, SlotCoord.y * csize + csize / 2);
		//====================================== Vertices ================= Colors ==================================== TextureC ========================================================================================
		vertices.push_back({ { glm::vec3(-offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(TextCoord.x - csize / 2, TextCoord.y + csize / 2) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
		vertices.push_back({ { glm::vec3(-offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(TextCoord.x - csize / 2, TextCoord.y - csize / 2) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
		vertices.push_back({ { glm::vec3(+offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(TextCoord.x + csize / 2, TextCoord.y - csize / 2) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
		vertices.push_back({ { glm::vec3(+offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(TextCoord.x + csize / 2, TextCoord.y + csize / 2) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
		//====================================== Indices
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
		//==================================================== Vertices Count
		vnum["Font"][Frame] = indices.size();

		//==================================================== Generate VAO
		glGenVertexArrays(1, &models["Font"][Frame]);
		//==================================================== Generate VBO
		glGenBuffers(1, &vbo["Font"][Frame]);
		//==================================================== Generate EBO
		glGenBuffers(1, &ebo["Font"][Frame]);

		//==================================================== Bind VAO
		glBindVertexArray(models["Font"][Frame]);
		//==================================================== Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo["Font"][Frame]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		//==================================================== Bind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo["Font"][Frame]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

		//==================================================== Attribute Location 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		//==================================================== Attribute Location 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//==================================================== Attribute Location 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//==================================================== Attribute Location 3
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		//==================================================== Attribute Location 4
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
		//==================================================== Attribute Location 5
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(14 * sizeof(GLfloat)));
		glEnableVertexAttribArray(5);

		//==================================================== Unbind VAO
		glBindVertexArray(0);
		//==================================================== Unbind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//==================================================== Unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void GLLoader::generate2DGeometry()
{
	framesNum["Box"] = 1;
	GLfloat offset = 0.5f;
	
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(-offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Indices
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	//==================================================== Vertices Count
	vnum["Box"][0] = indices.size();

	//==================================================== Generate VAO
	glGenVertexArrays(1, &models["Box"][0]);
	//==================================================== Generate VBO
	glGenBuffers(1, &vbo["Box"][0]);
	//==================================================== Generate EBO
	glGenBuffers(1, &ebo["Box"][0]);

	//==================================================== Bind VAO
	glBindVertexArray(models["Box"][0]);
	//==================================================== Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo["Box"][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	//==================================================== Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo["Box"][0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

	//==================================================== Attribute Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//==================================================== Attribute Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//==================================================== Attribute Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//==================================================== Attribute Location 3
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	//==================================================== Attribute Location 4
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	//==================================================== Attribute Location 5
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (void*)(14 * sizeof(GLfloat)));
	glEnableVertexAttribArray(5);

	//==================================================== Unbind VAO
	glBindVertexArray(0);
	//==================================================== Unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//==================================================== Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLLoader::generateTextureMap(std::vector<std::string>& Parts)
{
	typesNum[Parts[0]] = StringToInteger(Parts[1]);

	for (GLint Frame = 0; Frame < typesNum[Parts[0]]; Frame++)
	{
		GLint Width, Height, Channels;
		std::string Path = Parts[3] + Parts[0] + NumberToString(Frame) + Parts[2];

		//==================================================== Load Image File
		txt[Parts[0]][Frame] = stbi_load(Path.c_str(), &Width, &Height, &Channels, 4);

		if (!txt[Parts[0]][Frame])
		{
			std::cout << "Failed to load " << Path << std::endl;
		}
		else
		{
			//==================================================== Set OpenGL Profile		
			glGenTextures(1, &images[Parts[0]][Frame]);
			//==================================================== Bind Texture
			glBindTexture(GL_TEXTURE_2D, images[Parts[0]][Frame]);

			//==================================================== Sampler Wrapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//==================================================== Sampler Filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//==================================================== Store Image Data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, txt[Parts[0]][Frame]);
			glGenerateMipmap(GL_TEXTURE_2D);

			//==================================================== Unbind Texture
			glBindTexture(GL_TEXTURE_2D, 0);
			//==================================================== Free Image Data
			stbi_image_free(txt[Parts[0]][Frame]);
		}
	}
}
