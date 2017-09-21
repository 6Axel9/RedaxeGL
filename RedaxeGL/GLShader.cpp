#include "GLShader.h"
#include "Tools.h"

GLShader::GLShader(std::string Vertex,std::string Fragment)
{
	//==================================================== Create Program
	createProgram();
	//==================================================== Create/Compile/Attach Shaders
	createShaders(Vertex, Fragment);
	//==================================================== Link Program
	linkProgram();
	//==================================================== Link Uniforms
	linkUniforms();
}

GLShader::~GLShader()
{
	for (GLuint Type = 0; Type < shadersId.size();Type++)
	{
		//==================================================== Detach Shader
		glDetachShader(programId, shadersId[Type]);
		//==================================================== Delete Shader
		glDeleteShader(shadersId[Type]);
	}
	//==================================================== Delete Program
	glDeleteProgram(programId);
}

void GLShader::createProgram()
{
	//==================================================== Create Program
	programId = glCreateProgram();
	//==================================================== Validate
	if (programId == 0) 
	{     
		std::cout << "Failed to create program" << std::endl;
	}
}

void GLShader::createShaders(std::string Vertex,std::string Fragment)
{
	shadersId.resize(2); GLint result;
	//==================================================== Store Paths
	std::vector<std::string> shaders = { Vertex, Fragment };
	//==================================================== Create Vertex Shader
	shadersId[0] = glCreateShader(GL_VERTEX_SHADER);
	//==================================================== Create Fragment Shader
	shadersId[1] = glCreateShader(GL_FRAGMENT_SHADER);
	//==================================================== Validate
	if (shadersId[0] == 0 || shadersId[1] == 0) 
	{     
		std::cout << "Failed to create shaders" << std::endl;
	}
	for (GLuint Type = 0; Type < shaders.size();Type++)
	{
		std::string code = ReadShader(shaders[Type]);
		//==================================================== Convert Code
		const GLchar* compile = static_cast<const GLchar*>(code.c_str());
		//==================================================== Bind Source
		glShaderSource(shadersId[Type], 1, &compile, NULL);
		//==================================================== Compile Shader
		glCompileShader(shadersId[Type]);
		//==================================================== Query Result
		glGetShaderiv(shadersId[Type], GL_COMPILE_STATUS, &result);
		//==================================================== Validate
		if (result == GL_FALSE) 
		{ 
			std::cout << "Failed to compile shaders" << shaders[Type] << std::endl;
		}
		else
		{
			//==================================================== Attach Shader
			glAttachShader(programId, shadersId[Type]);
		}
	}
}

void GLShader::linkProgram()
{
	GLint result;
	//==================================================== Link program
	glLinkProgram(programId);
	//==================================================== Query Result
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	//==================================================== Validate
	if (result == GL_FALSE) 
	{
		std::cout << "Failed to link program" << std::endl;
	}
	//==================================================== Use Program
	glUseProgram(programId);
}

void GLShader::linkUniforms()
{
	//==================================================== Matrices
	uniformsId["modelIn"] = glGetUniformLocation(programId, "modelIn");
	uniformsId["viewIn"]  = glGetUniformLocation(programId, "viewIn");
	uniformsId["projIn"]  = glGetUniformLocation(programId, "projIn");
	//==================================================== Bools
	uniformsId["Textured"] = glGetUniformLocation(programId, "Textured");
	uniformsId["Mapped"]   = glGetUniformLocation(programId, "Mapped");
	uniformsId["Lit"]      = glGetUniformLocation(programId, "Lit");
	//==================================================== Light
	uniformsId["light.Ambient"]  = glGetUniformLocation(programId, "light.Ambient");
	uniformsId["light.Diffuse"]  = glGetUniformLocation(programId, "light.Diffuse");
	uniformsId["light.Specular"] = glGetUniformLocation(programId, "light.Specular");
	uniformsId["light.Position"] = glGetUniformLocation(programId, "light.Position");
	//==================================================== Materials
	uniformsId["material.Ambient"]   = glGetUniformLocation(programId, "material.Ambient");
	uniformsId["material.Diffuse"]   = glGetUniformLocation(programId, "material.Diffuse");
	uniformsId["material.Specular"]  = glGetUniformLocation(programId, "material.Specular");
	uniformsId["material.Shininess"] = glGetUniformLocation(programId, "material.Shininess");
	//==================================================== Textures
	uniformsId["txtmap.Diffuse"]  = glGetUniformLocation(programId, "txtmap.Diffuse");
	uniformsId["txtmap.Specular"] = glGetUniformLocation(programId, "txtmap.Specular");
	uniformsId["txtmap.Normal"]   = glGetUniformLocation(programId, "txtmap.Normal");
	//==================================================== Camera
	uniformsId["camera.Position"] = glGetUniformLocation(programId, "camera.Position");
}