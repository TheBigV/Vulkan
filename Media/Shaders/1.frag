#version 450

layout(set = 0, binding = 1) uniform sampler2D testTexture;

layout(set = 0, binding = 0) uniform Test
{
	vec4 color;
};

layout(location = 0) out vec4 oColor;

void main()
{
	vec2 tex = (gl_FragCoord.xy / 512.0f) * 2.0f - 0.5f;
	oColor = texture(testTexture, tex);
	//oColor = vec4(tex.x, tex.y, 0.0f, 1.0f);	//color;
	//oColor = vec4(0.0f, color.y, 0.0f, 1.0f);
}