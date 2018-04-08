#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include <map>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class GLLoader
{
public:

	//==================================================== Load Engine Assets
	GLLoader(std::string ModPath, std::string ImgPath, std::string SndPath);
	//==================================================== Unload Engine Assets 
	~GLLoader();

	//==================================================== Handlers
	std::map<GLint, GLuint>& FrameBuffers(std::string Tag) { return frameBuffers[Tag]; }
	std::map<GLint, GLuint>& DepthBuffers(std::string Tag) { return depthBuffers[Tag]; }
	std::map<GLint, GLuint>& RenderBuffers(std::string Tag) { return renderBuffers[Tag]; }
	std::map<GLint, GLuint>& Models(std::string Tag) { return models[Tag]; }
	std::map<GLint, GLuint>& Images(std::string Tag) { return images[Tag]; }
	std::map<GLint, GLuint>& Sounds(std::string Tag) { return sounds[Tag]; }
	std::map<GLint, GLuint>& VNum(std::string Tag)   { return vnum[Tag]; }
	glm::vec3& TerrainData() { return terrainData; }

private:

	//==================================================== Load All Frame Buffers
	void loadFrameBuffers();
	//==================================================== Load All Models/Animations
	void loadModels(std::string ModPath);
	//==================================================== Load All Textures/Fonts
	void loadImages(std::string ImgPath); 
	//==================================================== Load All Sounds/Effects
	void loadSounds(std::string SndPath);
	//==================================================== Fill Vertex Buffers
	void generate3DGeometry(std::vector<std::string>& Parts);
	void generateTerrainGeometry();
	void generateWaterGeometry();
	void generatePointGeometry();
	void generateCubeGeometry();
	void generateFontGeometry();
	void generate2DGeometry();
	//==================================================== Fill Texture Buffers
	void generateTextureMap(std::vector<std::string>& Parts);
	void generateCubeMap(std::vector<std::string>& Parts);
	//==================================================== Fill Frame Buffers
	void generateWaterReflection();
	void generateWaterRefraction();
	//==================================================== Properties
	std::map<std::string, std::map<GLint, GLuint>> frameBuffers;
	std::map<std::string, std::map<GLint, GLuint>> depthBuffers;
	std::map<std::string, std::map<GLint, GLuint>> renderBuffers;
	std::map<std::string, std::map<GLint, GLuint>> models;
	std::map<std::string, std::map<GLint, GLuint>> images;
	std::map<std::string, std::map<GLint, GLuint>> sounds;
	//==================================================== Sub-Buffers
	std::map<std::string, std::map<GLint, GLuint>> vbo;
	std::map<std::string, std::map<GLint, GLuint>> ebo;
	//==================================================== Other Info
	std::map<std::string, std::map<GLint, GLuint>> vnum;
	glm::vec3 terrainData;
	GLubyte* textureData;
};