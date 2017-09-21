#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <map>

class GLShader
{
public:

	//==================================================== Initialize OpenGL Shaders
	GLShader(std::string Vertex,std::string Fragment);
	//==================================================== Terminate OpenGL Shaders
	~GLShader();

	//==================================================== Handlers
	std::map<std::string, GLuint>& UniformsID() { return uniformsId; }

private:
	
	//==================================================== Create Program
	void createProgram();
	//==================================================== Create/Compile/Attach Shaders
	void createShaders(std::string Vertex,std::string Fragment);
	//==================================================== Link Program
	void linkProgram();
	//==================================================== Link Uniforms
	void linkUniforms();

	//==================================================== Properties
	std::map<std::string, GLuint> uniformsId;
	std::vector<GLuint> shadersId;
	GLuint programId;
};

