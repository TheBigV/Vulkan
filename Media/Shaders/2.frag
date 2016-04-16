#version 450

layout(set = 0, binding = 1) uniform sampler2D testTexture;

layout(set = 0, binding = 0) uniform Test
{
	vec4 color;
};

layout(location = 0) in vec2 pTex;
layout(location = 1) in vec3 pTan;
layout(location = 2) in vec3 pBin;
layout(location = 3) in vec3 pNor;

layout(location = 0) out vec4 oColor;

void main()
{
	//oColor = vec4(pTex, 0.0f, 1.0f);
	oColor = texture(testTexture, pTex);

	//oColor = vec4(pNor, 1.0f);
	
	//vec2 tex = (gl_FragCoord.xy / 512.0f) * 2.0f - 0.5f;
	//oColor = texture(testTexture, tex);
	//oColor = vec4(tex.x, tex.y, 0.0f, 1.0f);	//color;
	
	//oColor = color;

	//oColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}