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

out vec2 pass_TexCoord;

void main(void)
{	
	pass_TexCoord = in_TexCoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}