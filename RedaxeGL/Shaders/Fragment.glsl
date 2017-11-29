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
uniform bool MultiText;
uniform bool Mapped;
uniform bool Lit;

struct Light
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Position;
	vec3 Direction;
	float Attenuation;
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
};

struct Terrain
{
	sampler2D G0Diffuse;
	sampler2D G0Specular;
	sampler2D G0Normals;

	sampler2D G1Diffuse;
	sampler2D G1Specular;
	sampler2D G1Normals;

	sampler2D GNoise;
};

struct Camera
{
	vec3 Position;
};

uniform Light light;
uniform Material material;
uniform TextureMap txtmap;
uniform Terrain terrain;
uniform Camera camera;

void main(void)
{
	vec4 MapBlender = texture(terrain.GNoise, textureOut.st);
	float Intensity = 1.0 - clamp((MapBlender.r + MapBlender.g + MapBlender.b), 0.0, 1.0);

	if(Lit)
	{
		vec3 NormalPosition;
		vec3 VertexPosition  = (modelIn * vec4(positionOut, 1.0)).xyz;

		if(Mapped)
		{
			if(MultiText)
			{
				vec4 G0Normals = texture(terrain.G0Normals, textureOut.st * 8.0) * (1.0 - Intensity);
				vec4 G1Normals = texture(terrain.G1Normals, textureOut.st * 8.0) * Intensity;

				NormalPosition = normalize(normalize(vec4(G0Normals + G1Normals).rgb * 2.0 - 1.0) * TBN);
			}
			else
			{
				NormalPosition = normalize(normalize(texture(txtmap.Normals, textureOut.st).rgb * 2.0 - 1.0) * TBN);
			}
		}
		else
		{
			NormalPosition	 = normalize((mat4(transpose(inverse(modelIn))) * vec4(normalOut, 1.0)).xyz);
		}

		vec3 LightDirection;
		vec3 AmbientColor	 = light.Ambient * material.Ambient;

		if(light.Attenuation > 0)	
		{ 
			LightDirection = normalize(light.Position - VertexPosition);
		}
		else
		{
			LightDirection = normalize(light.Direction);
		}

		float LightIntensity = max(dot(LightDirection, NormalPosition), 0.0);

		vec3 DiffuseColor	 = light.Diffuse * material.Diffuse * LightIntensity;

		vec3 CameraDirection = normalize(camera.Position - VertexPosition);
		vec3 LightReflection = reflect(-LightDirection, NormalPosition);
		float SpecularTerm	 = pow(max(dot(CameraDirection, LightReflection), 0.0), material.Shininess);

		vec3 SpecularColor	 = light.Specular * material.Specular * SpecularTerm;

		if(Textured)
		{
			if(Mapped)
			{
				if(MultiText)
				{
					vec4 G0Diffuse  = texture(terrain.G0Diffuse,  textureOut.st * 8.0) * (1.0 - Intensity);
					vec4 G0Specular = texture(terrain.G0Specular, textureOut.st * 8.0) * (1.0 - Intensity);

					vec4 G1Diffuse  = texture(terrain.G1Diffuse,  textureOut.st * 8.0) * Intensity;
					vec4 G1Specular = texture(terrain.G1Specular, textureOut.st * 8.0) * Intensity;

					pixelColor = (	vec4(AmbientColor,  1.0) * (G0Diffuse  + G1Diffuse ) +
									vec4(DiffuseColor,  1.0) * (G0Diffuse  + G1Diffuse ) +
									vec4(SpecularColor, 1.0) * (G0Specular + G1Specular));

					pixelColor = vec4(pixelColor.rgb * clamp((VertexPosition.y + 3) / 6.0, 0.2, 1.0), 1.0);				
				}
				else
				{
					pixelColor = (	vec4(AmbientColor,  1.0) * texture(txtmap.Diffuse,  textureOut.st) +
									vec4(DiffuseColor,  1.0) * texture(txtmap.Diffuse,  textureOut.st) +
									vec4(SpecularColor, 1.0) * texture(txtmap.Specular, textureOut.st));
				}
			}
			else
			{
				pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0) * texture(txtmap.Diffuse,  textureOut.st);
			}
		}
		else
		{
			pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0);
		}
		if(light.Attenuation > 0)
		{
			pixelColor = vec4(pixelColor.xyz / pow((distance(light.Position, VertexPosition) / light.Attenuation), 2), 1.0);
		}
	}
	else
	{
		if(Textured)
		{
			pixelColor = vec4(colorOut, 1.0) * texture(txtmap.Diffuse, textureOut.st);
		}
		else
		{
			pixelColor = vec4(colorOut, 1.0);
		}
	}
}