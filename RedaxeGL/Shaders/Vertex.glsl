//vertex shader code 
#version 330 core 

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 colorIn;
layout (location = 2) in vec2 textureIn;
layout (location = 3) in vec3 normalIn;
layout (location = 4) in vec3 tangentIn;
layout (location = 5) in vec3 bitangentIn;

uniform mat4 modelIn;
uniform mat4 viewIn;
uniform mat4 projIn;

uniform vec4 clipPlane;

out vec4 clipSpace;
out vec3 positionOut;
out vec3 colorOut;
out vec2 textureOut;
out vec3 normalOut;

out mat3 TBN;

void main(void)
{
	positionOut	= positionIn;
	colorOut	= colorIn;
	textureOut	= textureIn;
	normalOut	= normalIn;

	vec3 T = normalize(vec3(modelIn * vec4(tangentIn,   0.0)));
	vec3 B = normalize(vec3(modelIn * vec4(bitangentIn, 0.0)));
	vec3 N = normalize(vec3(modelIn * vec4(normalIn,    0.0)));

	TBN = transpose(mat3(T, B, N));

	gl_ClipDistance[0] = dot(modelIn * vec4(positionIn, 1.0), clipPlane);

	clipSpace = projIn * viewIn * modelIn * vec4(positionIn, 1.0);

	gl_Position = clipSpace;
}