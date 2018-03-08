//fragment shader code 
#version 330 core

in vec4 clipSpace;
in vec3 positionOut;
in vec3 colorOut;
in vec2 textureOut;
in vec3 normalOut;

in mat3 TBN;

out vec4 pixelColor;

uniform mat4 modelIn;

uniform bool DiffuseMap;
uniform bool SpecularMap;
uniform bool NormalMap;
uniform bool Shaded;

uniform bool TerrainShader;
uniform bool WaterShader;
uniform bool ShadowShader;

uniform float ElapsedTime;
uniform float DeltaTime;

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

struct TerrainMap
{
	sampler2D G0Diffuse;
	sampler2D G0Specular;
	sampler2D G0Normals;
	sampler2D G1Diffuse;
	sampler2D G1Specular;
	sampler2D G1Normals;
	sampler2D GNoise;
};

struct WaterMap
{
	sampler2D Reflection;
	sampler2D Refraction;
	sampler2D Distortion;
	sampler2D Normals;
};

struct Camera
{
	vec3 Position;
};

uniform Light light;
uniform Material material;
uniform TextureMap txtmap;
uniform TerrainMap terrain;
uniform WaterMap water;
uniform Camera camera;

void main(void)
{
	//==================================================== Shading Process
	if(Shaded)
	{
		vec3 LightDirection;
		vec3 NormalPosition;
		float TerrainIntensity;
		//==================================================== Convert To Device Space
		vec2 DeviceSpace = (clipSpace.xy / clipSpace.w) / 2 + 0.5;
		//==================================================== Convert To Model Space
		vec3 VertexPosition  = (modelIn * vec4(positionOut, 1.0)).xyz;
		//==================================================== Camera View Direction
		vec3 CameraDirection = normalize(camera.Position - VertexPosition);
		//==================================================== Sample Noise Texture
		if(TerrainShader)
		{
			vec4 MapBlender = normalize(texture(terrain.GNoise, textureOut.st));
			TerrainIntensity = 1.0 - (MapBlender.r + MapBlender.g + MapBlender.b);
		}
		//==================================================== Convert Normal Maps To Normal Directions
		if(NormalMap)
		{
			//==================================================== Terrain Multiple Normal Maps Blending
			if(TerrainShader)
			{
				float Tiling = 24.0;

				vec4 G0Normals = texture(terrain.G0Normals, textureOut.st * Tiling);
				vec4 G1Normals = texture(terrain.G1Normals, textureOut.st * Tiling);
				
				vec3 TerrainNormalsG0 = vec4(G0Normals).rgb * 2.0 - 1.0;
				vec3 TerrainNormalsG1 = vec4(G1Normals).rgb * 2.0 - 1.0;

				vec3 TerrainNormals = vec3(TerrainNormalsG0.xy * TerrainNormalsG1.z + 
										   TerrainNormalsG1.xy * TerrainNormalsG0.z, TerrainNormalsG0.z * TerrainNormalsG1.z); 

				NormalPosition = normalize(normalize(TerrainNormals) * TBN);
			}
			//==================================================== 3D Object Normal Map
			else
			{
				NormalPosition = normalize(normalize(texture(txtmap.Normals, textureOut.st).rgb * 2.0 - 1.0) * TBN);
			}
		}
		//==================================================== Standard Vertices Normals
		else 
		{	
			NormalPosition = normalize((mat4(transpose(inverse(modelIn))) * vec4(normalOut, 1.0)).xyz); 
		}
		//==================================================== Point Light Direction
		if(light.Attenuation != 0)	
		{ 
			LightDirection = normalize(light.Position - VertexPosition); 
		}
		//==================================================== Directional Light
		else
		{ 
			LightDirection = normalize(light.Direction); 
		}
		//==================================================== Light Intensity
		float LightIntensity = max(dot(LightDirection, NormalPosition), 0.0);
		//==================================================== Light Reflection
		vec3 LightReflection = reflect(-LightDirection, NormalPosition);
		//==================================================== Light Specularity
		float SpecularTerm	 = pow(max(dot(CameraDirection, LightReflection), 0.0), material.Shininess);
		//==================================================== Ambient Lighting
		vec3 AmbientColor	 = light.Ambient * material.Ambient;
		//==================================================== Diffuse Lighting
		vec3 DiffuseColor	 = light.Diffuse * material.Diffuse * LightIntensity;
		//==================================================== Specular Lighting
		vec3 SpecularColor	 = light.Specular * material.Specular * SpecularTerm;

		if(DiffuseMap)
		{
			//==================================================== Sample Reflection & Refraction Map
			if(WaterShader)
			{
				float OffSet = ElapsedTime / 1000.0;
				float WaveIntensity = 0.01;
				float Tiling = 24.0;

				vec2 DistortionBlend0 = texture(water.Distortion, vec2(textureOut.x + OffSet, -textureOut.y + OffSet) * Tiling).rg * 2.0 - 1.0;
				vec2 DistortionBlend1 = texture(water.Distortion, vec2(-textureOut.x + OffSet, textureOut.y + OffSet) * Tiling).rg * 2.0 - 1.0;

				vec2 DistortionFactor = DistortionBlend0 + DistortionBlend1;
			
				vec2 ReflectionCoords = vec2(DeviceSpace.x, -DeviceSpace.y) + DistortionFactor * WaveIntensity;
				vec2 RefractionCoords = vec2(DeviceSpace.x, DeviceSpace.y)  + DistortionFactor * WaveIntensity;

				ReflectionCoords.x = clamp(ReflectionCoords.x, 0.001, 0.999);
				ReflectionCoords.y = clamp(ReflectionCoords.y, -0.999, -0.001);
				RefractionCoords   = clamp(RefractionCoords, 0.001, 0.999);

				vec4 ReflectionColor = texture(water.Reflection, ReflectionCoords);
				vec4 RefractionColor = texture(water.Refraction, RefractionCoords);
				vec4 WaterDiffuse = mix(ReflectionColor, RefractionColor, 0.5);

				pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0) * WaterDiffuse;
			}
			//==================================================== Sample Terrain Diffuse Maps
			if(TerrainShader)
			{
				float Tiling = 24.0;

				vec4 G0Diffuse  = texture(terrain.G0Diffuse,  textureOut.st * Tiling) * (1.0 - TerrainIntensity);
				vec4 G1Diffuse  = texture(terrain.G1Diffuse,  textureOut.st * Tiling) * TerrainIntensity;
				vec4 TerrainDiffuse = mix(G0Diffuse, G1Diffuse, 0.5);

				pixelColor = vec4(AmbientColor + DiffuseColor,  1.0) * TerrainDiffuse;
				//==================================================== Sample Terrain Specular Maps
				if(SpecularMap)
				{
					vec4 G0Specular = texture(terrain.G0Specular, textureOut.st * Tiling) * (1.0 - TerrainIntensity);
					vec4 G1Specular = texture(terrain.G1Specular, textureOut.st * Tiling) * TerrainIntensity;
					vec4 TerrainSpecular = mix(G0Specular, G1Specular, 0.5);

					pixelColor += vec4(SpecularColor, 1.0) * TerrainSpecular;
				}
				//==================================================== Use Diffuse As Specular
				else
				{
					pixelColor += vec4(SpecularColor, 1.0) * TerrainDiffuse;
				}
			}
			if(!WaterShader && !TerrainShader)
			{
				pixelColor = vec4(AmbientColor + DiffuseColor,  1.0) * texture(txtmap.Diffuse,  textureOut.st);

				if(SpecularMap)
				{
					pixelColor += vec4(SpecularColor, 1.0) * texture(txtmap.Specular, textureOut.st);
				}
				else
				{
					pixelColor += vec4(SpecularColor, 1.0) * texture(txtmap.Diffuse, textureOut.st);
				}
			}
		}
		else
		{
			pixelColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 0.5);	
		}
		if(light.Attenuation != 0)
		{
			pixelColor = vec4(pixelColor.xyz / pow((distance(light.Position, VertexPosition) / light.Attenuation), 2), 1.0);
		}
	}
	else
	{
		if(DiffuseMap)
		{
			pixelColor = vec4(colorOut, 1.0) * texture(txtmap.Diffuse, textureOut.st);
		}
		else
		{
			pixelColor = vec4(colorOut, 0.5);
		}
	}
}