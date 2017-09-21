//fragment shader code 
#version 330 core

in vec3 positionOut;
in vec3 colorOut;
in vec2 textureOut;
in vec3 normalOut;

in mat3 TBN;

out vec4 pixelColor;

uniform mat4 modelIn;

uniform bool Textured;
uniform bool Mapped;
uniform bool Mapped2;
uniform bool Lit;

struct Light
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Position;
};

struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

struct TextureMap
{
	sampler2D Diffuse;
	sampler2D Specular;
	sampler2D Normals;
	sampler2D Normals2;
};

struct Camera
{
	vec3 Position;
};

uniform Light light;
uniform Material material;
uniform TextureMap txtmap;
uniform Camera camera;

uniform float ElapsedTime;
uniform float GameSpeed;

void main(void)
{
	//if(Lit)
	//{
	//	vec3 NormalPosition;
	//	vec3 VertexPosition = (modelIn * vec4(positionOut, 1.0)).xyz;

	//	if(Mapped)
	//	{
	//		if(!Mapped2)
	//		{
	//			NormalPosition  = normalize(normalize(texture(txtmap.Normals, textureOut.st).rgb * 2.0 - 1.0) * TBN);
	//		}
	//		else
	//		{
	//			float WaveFrequency = 0.5;
	//			float WaveAmplitude = 10;

	//			vec2 TextureCoord = textureOut.st;
	//			TextureCoord.x = sin(TextureCoord.x * WaveFrequency) * WaveAmplitude;
	//			TextureCoord.y = sin(TextureCoord.y * WaveFrequency) * WaveAmplitude;

	//			NormalPosition  = normalize(normalize((texture(txtmap.Normals,  vec2(TextureCoord.s+ElapsedTime/20+GameSpeed,TextureCoord.t)).rgb * 2.0 - 1.0) +
	//												  (texture(txtmap.Normals2, vec2(TextureCoord.s-ElapsedTime/25+GameSpeed,TextureCoord.t)).rgb * 2.0 - 1.0)) * TBN);
	//		}
	//	}
	//	else
	//	{
	//		NormalPosition	 = normalize((mat4(transpose(inverse(modelIn))) * vec4(normalOut, 1.0)).xyz);
	//	}

	//	vec3 AmbientColor	 = light.Ambient * material.Ambient;

	//	vec3 LightDirection  = normalize(light.Position - VertexPosition);
	//	float LightIntensity = max(dot(LightDirection, NormalPosition), 0.0);

	//	vec3 DiffuseColor	 = light.Diffuse * material.Diffuse * LightIntensity;

	//	vec3 CameraDirection = normalize(camera.Position - VertexPosition);
	//	vec3 LightReflection = reflect(-LightDirection, NormalPosition);
	//	float SpecularTerm	 = pow(max(dot(CameraDirection, LightReflection), 0.0), material.Shininess);

	//	vec3 SpecularColor	 = light.Specular * material.Specular * SpecularTerm;

	//	if(Textured)
	//	{
	//		if(Mapped)
	//		{
	//			if(Mapped2)
	//			{
	//				float WaveFrequency = 0.5;
	//				float WaveAmplitude = 10;
	//				vec2 OffSet = vec2(0,2.65);

	//				vec2 TextureCoord = textureOut.st;
	//				TextureCoord.x = (sin(TextureCoord.x * WaveFrequency+OffSet.x)) * WaveAmplitude;
	//				TextureCoord.y = (sin(TextureCoord.y * WaveFrequency+OffSet.y)) * WaveAmplitude;

	//				pixelColor = (	vec4(AmbientColor,  1.0) * texture(txtmap.Diffuse,  vec2(TextureCoord.s-ElapsedTime/25+GameSpeed,TextureCoord.t)) +
	//								vec4(DiffuseColor,  1.0) * texture(txtmap.Diffuse,  vec2(TextureCoord.s-ElapsedTime/25+GameSpeed,TextureCoord.t)) +
	//								vec4(SpecularColor, 1.0) * texture(txtmap.Specular, vec2(TextureCoord.s-ElapsedTime/25+GameSpeed,TextureCoord.t)));
	//			}
	//			else
	//			{
	//				pixelColor = (	vec4(AmbientColor,  1.0) * texture(txtmap.Diffuse,  textureOut.st) +
	//								vec4(DiffuseColor,  1.0) * texture(txtmap.Diffuse,  textureOut.st) +
	//								vec4(SpecularColor, 1.0) * texture(txtmap.Specular, textureOut.st));
	//			}
	//				pixelColor =	vec4(pixelColor.xyz / pow((distance(light.Position, VertexPosition) / 60), 3), 1.0);
	//		}
	//		else
	//		{
	//			pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0) * texture(txtmap.Diffuse,  textureOut.st);
	//		}
	//	}
	//	else
	//	{
	//		if(Mapped)
	//		{
	//			pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0) * texture(txtmap.Normals,  textureOut.st);
	//		}
	//		else
	//		{
	//			pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0);
	//		}
	//	}
	//}
	//else
	//{
	//	if(Textured)
	//	{
	//		pixelColor = vec4(colorOut, 1.0) * texture(txtmap.Diffuse, textureOut.st);
	//	}
	//	else
	//	{
			pixelColor = vec4(colorOut, 1.0);	
		}
	//}
//}