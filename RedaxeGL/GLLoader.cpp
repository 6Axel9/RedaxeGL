#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GLLoader.h"
#include "Engine.h"
#include "Tools.h"
#include <postprocess.h>
#include <cimport.h>
#include <scene.h>
#include <time.h>


GLLoader::GLLoader(std::string ModPath, std::string ImgPath, std::string SndPath)
{
	loadFrameBuffers();
	//==================================================== Load Model Assets
	loadModels(ModPath);
	//==================================================== Load Image Assets
	loadImages(ImgPath);
}

GLLoader::~GLLoader()
{
	//==================================================== Delete FBO
	for (auto const &Tag : frameBuffers)
	{
		for (auto const &Frame : Tag.second)
		{
			glDeleteFramebuffers(1, &Frame.second);
		}
	}
	//==================================================== Delete Depth Attachment
	for (auto const &Tag : depthBuffers)
	{
		for (auto const &Frame : Tag.second)
		{
			glDeleteTextures(1, &Frame.second);
		}
	}
	//==================================================== Delete Render Buffer
	for (auto const &Tag : renderBuffers)
	{
		for (auto const &Frame : Tag.second)
		{
			glDeleteRenderbuffers(1, &Frame.second);
		}
	}
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

void GLLoader::loadFrameBuffers()
{
	generateWaterReflection();
	generateWaterRefraction();
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
	generateWaterGeometry();
	//==================================================== Load Point
	generatePointGeometry();
	//==================================================== Load Cube
	generateCubeGeometry();
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
				if (parts[4] != "C")
				{
					generateTextureMap(parts);
				}
				else
				{
					generateCubeMap(parts);
				}
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
	GLint frames = StringToInteger(Parts[1]);

	for (GLint Frame = 0; Frame < frames; Frame++)
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
	GLfloat lowest = 0.0f;
	GLfloat highest = 0.0f;
	GLfloat average = 0.0f;
	GLfloat amplitude = StringToFloat(Engine::Settings()["Terrain.amplitude"]);
	GLfloat roughness = StringToFloat(Engine::Settings()["Terrain.roughness"]);
	GLint octaves = StringToInteger(Engine::Settings()["Terrain.octaves"]);
	GLint vcount = StringToInteger(Engine::Settings()["Terrain.vertices"]);;
	GLint tsize = StringToInteger(Engine::Settings()["Terrain.size"]);;

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
			//==================================================== Store Terrain Top/Bottom Picks & Average
			if (lowest > Height) { lowest = Height; }
			if (highest < Height) { highest = Height; }
			average += Height;
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

			//==================================================== Indices
			if (posX < vcount - 1 && posY < vcount - 1)
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
	}
	//==================================================== Store Captured Data
	terrainData = glm::vec3(lowest, highest, average / (vcount * vcount));
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

void GLLoader::generateWaterGeometry()
{
	GLfloat amplitude = 1;
	GLfloat roughness = 0;
	GLint octaves = 0;
	GLint vcount = 128;
	GLint tsize = 128;

	GLuint seed = (unsigned int)time(NULL);

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
				{ Bitangent } });
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
	vnum["Water"][0] = indices.size();

	//==================================================== Generate VAO
	glGenVertexArrays(1, &models["Water"][0]);
	//==================================================== Generate VBO
	glGenBuffers(1, &vbo["Water"][0]);
	//==================================================== Generate EBO
	glGenBuffers(1, &ebo["Water"][0]);

	//==================================================== Bind VAO
	glBindVertexArray(models["Water"][0]);
	//==================================================== Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo["Water"][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	//==================================================== Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo["Water"][0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_DYNAMIC_DRAW);

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

void GLLoader::generatePointGeometry()
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//======================== Vertices =========== Colors =========== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(0.0f) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f,0.0f,-1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Indices
	indices.push_back(0);
	//==================================================== Vertices Count
	vnum["Point"][0] = indices.size();

	//==================================================== Generate VAO
	glGenVertexArrays(1, &models["Point"][0]);
	//==================================================== Generate VBO
	glGenBuffers(1, &vbo["Point"][0]);
	//==================================================== Generate EBO
	glGenBuffers(1, &ebo["Point"][0]);

	//==================================================== Bind VAO
	glBindVertexArray(models["Point"][0]);
	//==================================================== Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo["Point"][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	//==================================================== Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo["Point"][0]);
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

void GLLoader::generateCubeGeometry()
{
	GLint frames = 1;
	GLfloat offset = 0.5f;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(-offset, +offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, -offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, -offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(-offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, -offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, +offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(+offset, -offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(-offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(-offset, +offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, +offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	//====================================== Vertices ================= Colors ============== TextureC ========================================================================
	vertices.push_back({ { glm::vec3(+offset, -offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, -offset, -offset) },{ glm::vec3(1.0f) },{ glm::vec2(0.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(-offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	vertices.push_back({ { glm::vec3(+offset, -offset, +offset) },{ glm::vec3(1.0f) },{ glm::vec2(1.0f, 1.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) },{ glm::vec3(0.0f) } });
	
	for (GLint Face = 0; Face < 6; Face++)
	{
		//====================================== Indices
		indices.push_back(0 + 4 * Face);
		indices.push_back(1 + 4 * Face);
		indices.push_back(2 + 4 * Face);
		indices.push_back(2 + 4 * Face);
		indices.push_back(3 + 4 * Face);
		indices.push_back(0 + 4 * Face);
	}
	//==================================================== Vertices Count
	vnum["Cube"][0] = indices.size();

	//==================================================== Generate VAO
	glGenVertexArrays(1, &models["Cube"][0]);
	//==================================================== Generate VBO
	glGenBuffers(1, &vbo["Cube"][0]);
	//==================================================== Generate EBO
	glGenBuffers(1, &ebo["Cube"][0]);

	//==================================================== Bind VAO
	glBindVertexArray(models["Cube"][0]);
	//==================================================== Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo["Cube"][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	//==================================================== Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo["Cube"][0]);
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
	GLint frames = 16 * 16;
	GLfloat csize = 1.0f / 16.0f;
	GLfloat offset = 0.5f;
	
	for (GLint Frame = 0; Frame < frames; Frame++)
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
	GLint frames = 1;
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
	GLint types = StringToInteger(Parts[1]);

	for (GLint Frame = 0; Frame < types; Frame++)
	{
		if (Parts[4] == "N")
		{
			Frame = types - 1;
		}

		GLint Width, Height, Channels;
		std::string Path = Parts[3] + Parts[0] + NumberToString(Frame) + Parts[2];

		//==================================================== Load Image File
		textureData = stbi_load(Path.c_str(), &Width, &Height, &Channels, 4);

		if (!textureData)
		{
			std::cout << "Failed to load " << Path << std::endl;
		}
		else
		{
			//==================================================== Generate Texture		
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);

			//==================================================== Unbind Texture
			glBindTexture(GL_TEXTURE_2D, 0);
			//==================================================== Free Image Data
			stbi_image_free(textureData);
		}
	}
}

void GLLoader::generateCubeMap(std::vector<std::string>& Parts)
{
	GLint type = StringToInteger(Parts[1]) - 1;

	//==================================================== Generate Texture		
	glGenTextures(1, &images[Parts[0]][type]);
	//==================================================== Bind Texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, images[Parts[0]][type]);

	//==================================================== Sampler Wrapping
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//==================================================== Sampler Filter
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (GLint Face = 0; Face < 6; Face++)
	{
		GLint Width, Height, Channels;
		std::string Path = Parts[3] + Parts[0] + NumberToString(Face) + Parts[2];

		//==================================================== Load Image File
		textureData = stbi_load(Path.c_str(), &Width, &Height, &Channels, 4);

		if (!textureData)
		{
			std::cout << "Failed to load " << Path << std::endl;
		}
		else
		{
			//==================================================== Store Image Data
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

			//==================================================== Free Image Data
			stbi_image_free(textureData);
		}
	}
	//==================================================== Unbind Texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GLLoader::generateWaterReflection()
{
	//==================================================== Generate Frame Buffer
	glGenFramebuffers(1, &frameBuffers["Water"][0]);
	//==================================================== Bind Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers["Water"][0]);
	//==================================================== Define Draw Locations
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//==================================================== Generate Texture
	glGenTextures(1, &images["Water"][0]);
	//==================================================== Bind Texture
	glBindTexture(GL_TEXTURE_2D, images["Water"][0]);
	//==================================================== Sampler Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//==================================================== Store Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)Engine::Screen()->Size().x, (GLsizei)Engine::Screen()->Size().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	//==================================================== Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//==================================================== Generate Render Buffer
	glGenRenderbuffers(1, &renderBuffers["Water"][0]);
	//==================================================== Bind Render Buffer
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffers["Water"][0]);
	//==================================================== Define Storage Type
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)Engine::Screen()->Size().x, (GLsizei)Engine::Screen()->Size().y);
	//==================================================== Unbind Render Buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//==================================================== Attach Color Buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, images["Water"][0], 0);
	//==================================================== Attach Render Buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffers["Water"][0]);

	//==================================================== Bind Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLLoader::generateWaterRefraction()
{
	//==================================================== Generate Frame Buffer
	glGenFramebuffers(1, &frameBuffers["Water"][1]);
	//==================================================== Bind Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers["Water"][1]);
	//==================================================== Define Draw Locations
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//==================================================== Generate Texture
	glGenTextures(1, &images["Water"][1]);
	//==================================================== Bind Texture
	glBindTexture(GL_TEXTURE_2D, images["Water"][1]);
	//==================================================== Sampler Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//==================================================== Store Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)Engine::Screen()->Size().x, 
										   (GLsizei)Engine::Screen()->Size().y, 
								0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	//==================================================== Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//==================================================== Generate Texture
	glGenTextures(1, &depthBuffers["Water"][1]);
	//==================================================== Bind Texture
	glBindTexture(GL_TEXTURE_2D, depthBuffers["Water"][1]);
	//==================================================== Sampler Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//==================================================== Store Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, (GLsizei)Engine::Screen()->Size().x, 
														 (GLsizei)Engine::Screen()->Size().y, 
								0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	//==================================================== Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//==================================================== Attach Color Buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, images["Water"][1], 0);
	//==================================================== Attach Depth Buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuffers["Water"][1], 0);

	//==================================================== Bind Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
