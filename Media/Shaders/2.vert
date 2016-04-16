#version 450

layout(set = 0, binding = 2, column_major) uniform Model
{
	mat4 modelMatrix;
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTex;
layout(location = 2) in vec3 vTan;
layout(location = 3) in vec3 vBin;
layout(location = 4) in vec3 vNor;

layout(location = 0) out vec2 pTex;
layout(location = 1) out vec3 pTan;
layout(location = 2) out vec3 pBin;
layout(location = 3) out vec3 pNor;

void main()
{
	gl_Position = vec4(vPos, 1.0f) * modelMatrix;
	gl_Position.xyz /= gl_Position.w;
	gl_Position.y *= -1.0f;
	
	pTex = vTex;
	pTan = vTan;
	pBin = vBin;
	pNor = vNor;
}