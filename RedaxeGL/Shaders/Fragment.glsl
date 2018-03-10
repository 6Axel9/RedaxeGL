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
		vec4 DiffuseTexture;
		vec4 SpecularTexture;
		//==================================================== Convert To Device Space
		vec2 DeviceSpace = (clipSpace.xy / clipSpace.w) / 2 + 0.5;
		//==================================================== Convert To Model Space
		vec3 FragmentPosition  = (modelIn * vec4(positionOut, 1.0)).xyz;
		//==================================================== Camera View Direction
		vec3 CameraDirection = normalize(camera.Position - FragmentPosition);
		//==================================================== Water Maps Blending & Distortion
		if(WaterShader)
		{
			float OffSet = ElapsedTime / 1000.0;
			float DistortionIntensity = 0.1;
			float DistortionTiling = 24.0;
			float WaveIntensity = 0.025;
			//==================================================== Sample Distortion Map
			vec2 DistortionBlend = texture(water.Distortion, vec2(textureOut.x + OffSet, textureOut.y) * DistortionTiling).rg * DistortionIntensity;
				 DistortionBlend = textureOut + vec2(DistortionBlend.x, DistortionBlend.y + OffSet);

			vec2 DistortionFactor = (texture(water.Distortion, DistortionBlend).rg * 2.0 - 1.0) * WaveIntensity;
			//==================================================== Sample Normal Map
			NormalPosition = normalize(normalize(texture(water.Normals, DistortionBlend).rgb * 2.0 - 1.0) * TBN);
			//==================================================== Sample Reflection/Refraction Maps
			vec2 ReflectionCoords = vec2(DeviceSpace.x, -DeviceSpace.y) + DistortionFactor;
			vec2 RefractionCoords = vec2(DeviceSpace.x, DeviceSpace.y)  + DistortionFactor;

			ReflectionCoords.x = clamp(ReflectionCoords.x, 0.001, 0.999);
			ReflectionCoords.y = clamp(ReflectionCoords.y, -0.999, -0.001);
			RefractionCoords   = clamp(RefractionCoords, 0.001, 0.999);

			vec4 ReflectionColor = texture(water.Reflection, ReflectionCoords);
			vec4 RefractionColor = texture(water.Refraction, RefractionCoords);
			float RefractiveTerm = dot(CameraDirection, normalOut);

			DiffuseTexture = mix(ReflectionColor, RefractionColor, RefractiveTerm);
			SpecularTexture = DiffuseTexture;
		}
		//==================================================== Terrain Multiple Maps Blending
		if(TerrainShader)
		{
			float TerrainTiling = 24.0;
			float NormalTiling = 8.0;
			//==================================================== Sample Noise Blend Texture
			vec4 MapBlender = normalize(texture(terrain.GNoise, textureOut.st));
			float TerrainIntensity = 1.0 - (MapBlender.r + MapBlender.g + MapBlender.b);
			//==================================================== Sample Normal Maps
			vec4 G0Normals = texture(terrain.G0Normals, textureOut.st * NormalTiling);
			vec4 G1Normals = texture(terrain.G1Normals, textureOut.st * NormalTiling);
			
			vec3 TerrainNormalsG0 = vec4(G0Normals).rgb * 2.0 - 1.0;
			vec3 TerrainNormalsG1 = vec4(G1Normals).rgb * 2.0 - 1.0;

			vec3 TerrainNormals = vec3(TerrainNormalsG0.xy * TerrainNormalsG1.z + 
									   TerrainNormalsG1.xy * TerrainNormalsG0.z, TerrainNormalsG0.z * TerrainNormalsG1.z); 

			NormalPosition = normalize(normalize(TerrainNormals) * TBN);
			//==================================================== Sample Diffuse Maps
			vec4 G0Diffuse  = texture(terrain.G0Diffuse,  textureOut.st * TerrainTiling) * (1.0 - TerrainIntensity);
			vec4 G1Diffuse  = texture(terrain.G1Diffuse,  textureOut.st * TerrainTiling) * TerrainIntensity;
			
			DiffuseTexture = mix(G0Diffuse, G1Diffuse, 0.5);
			//==================================================== Sample Specular Maps
			vec4 G0Specular = texture(terrain.G0Specular, textureOut.st * TerrainTiling) * (1.0 - TerrainIntensity);
			vec4 G1Specular = texture(terrain.G1Specular, textureOut.st * TerrainTiling) * TerrainIntensity;
			
			SpecularTexture = mix(G0Specular, G1Specular, 0.5);
		}
		//==================================================== Sample 3D Object Maps Blending
		if(!WaterShader && !TerrainShader)
		{
			//==================================================== Sample Normal Map
			NormalPosition = normalize(normalize(texture(txtmap.Normals, textureOut.st).rgb * 2.0 - 1.0) * TBN);
			//==================================================== Sample Diffuse Map
			DiffuseTexture = texture(txtmap.Diffuse,  textureOut.st);
			//==================================================== Sample Specular Map
			SpecularTexture = texture(txtmap.Specular,  textureOut.st);
		}
		//==================================================== Standard Vertices Normals
		if(!NormalMap)
		{ 	
			NormalPosition = normalize((mat4(transpose(inverse(modelIn))) * vec4(normalOut, 1.0)).xyz); 
		}
		//==================================================== Standard Diffuse Color
		if(!DiffuseMap)
		{ 	
			DiffuseTexture = vec4(material.Ambient + material.Diffuse + material.Specular, 1.0); 
		}
		//==================================================== Standard Specular Color
		if(!SpecularMap)
		{ 	
			SpecularTexture = DiffuseTexture; 
		}
		//==================================================== Point Light Direction
		if(light.Attenuation != 0)	
		{ 
			LightDirection = normalize(light.Position - FragmentPosition); 
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

		pixelColor = vec4(AmbientColor,  1.0) * DiffuseTexture +
					 vec4(DiffuseColor,  1.0) * DiffuseTexture +
					 vec4(SpecularColor, 1.0) * SpecularTexture;

		if(light.Attenuation != 0)
		{
			pixelColor = vec4(pixelColor.xyz / pow((distance(light.Position, FragmentPosition) / light.Attenuation), 2), 1.0);
		}
	}
	else
	{
		pixelColor = vec4(colorOut, 1.0) * texture(txtmap.Diffuse, textureOut.st);
	}
}