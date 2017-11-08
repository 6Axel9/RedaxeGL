#include <iostream>
#include <time.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <map>

//--------------------------------------------------- Cut String Ending
static std::string CutPast(std::string& SubString, std::string::iterator Start)
{
	std::string Temp = SubString;
	Temp.erase(Temp.begin()+1, Temp.end());
	return Temp;
}
//--------------------------------------------------- Read From Script
static void ScriptDecript(std::string& String, std::map<std::string,std::string>& Container)
{
	int Tabs = String.find("\t");
	while (Tabs != std::string::npos)
	{
		String.erase(Tabs,1);
		Tabs = String.find("\t");
	}
	int Last  = String.find(";");
	if (Last != std::string::npos)
	{
		int First  = String.find("=");
		int Lenght = Last - (First+2);
		Container[String.substr(0, First)] = String.substr(First+2, Lenght);
	}
}
//--------------------------------------------------- Read From Script
static void ImportDecript(std::string& String, std::vector<std::string>& Container)
{
	Container.clear();

	int Tabs = String.find("\t");
	while (Tabs != std::string::npos)
	{
		String.erase(Tabs,1);
		Tabs = String.find("\t");
	}
	for (int i = 0; i < 4; i++)
	{
		int Last  = String.find(";");
		if (Last != std::string::npos)
		{
			int First  = String.find("=");
			int Lenght = Last - (First+2);
			Container.push_back(String.substr(First + 2, Lenght));
			String.erase(Last,1);
			String.erase(First,1);
		}
	}
}
//--------------------------------------------------- Read/Store Shaders Spir-V
static std::vector<char> ReadShaderBin(const std::string& filename) 
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file) 
	{     
		std::cout << "Failed to Read " << filename << std::endl;
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}
//--------------------------------------------------- Read/Store Shaders Glsl
static std::string ReadShader(const std::string& filename) 
{
	std::string lineText = ""; 
	std::string finalString = "";

	std::ifstream file(filename);

	if (!file) 
	{     
		std::cout << "Failed to Read " << filename << std::endl;
	} 
	while (!file.eof()) 
	{ 
		getline(file, lineText);     
		finalString += lineText + "\n"; 
	}

	file.close();

	return finalString;
}

//--------------------------------------------------- Calculate Delta Time
static float Noise(int X, int Y)
{
	srand(time(0));
	srand(X  + Y  + rand());
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return random;
}
//--------------------------------------------------- Calcolate Delta Time
static float DeltaTime(std::chrono::time_point<std::chrono::steady_clock> Start, std::chrono::time_point<std::chrono::steady_clock> End)
{
	float DT = std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() / 1000000.0f;

	if (DT > 0.0f) { return DT; }
	else { return 0; }
}
//--------------------------------------------------- Sort By Size M to m
static bool BiggerThan(float First, float Second)
{
	return First > Second;
}
//--------------------------------------------------- Convert String To Int
static int StringToInteger(const std::string& String)
{
	return atoi(String.c_str());
}
//--------------------------------------------------- Convert String To Float
static float StringToFloat(const std::string& String)
{
	return (float) atof(String.c_str());
}

//--------------------------------------------------- Convert Number To String
static std::string NumberToString(int Numb)
{
	std::stringstream str;
	str << Numb;
	return str.str();
}