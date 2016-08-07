#version 400 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Tangent;
layout(location=3) in vec3 in_Bitangent;
layout(location=4) in vec2 in_TexCoord;
layout(location=5) in vec4 in_Color;

out vec4 pass_Color;
out vec3 pass_Position;
out vec2 pass_TexCoord;

void main(void)
{	
	pass_Position = in_Position;
	pass_Color = in_Color;
	pass_TexCoord = in_TexCoord;
	
	// Subpixel shift
	vec4 pos = vec4(in_Position, 1.0);
	pos.xy = (in_Position.xy * vec2(0.5, 0.5)) + vec2(0.5, 0.5);
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
}