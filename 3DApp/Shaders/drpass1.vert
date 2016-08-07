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
out vec3 pass_Normal;
out vec3 pass_Position;
out vec2 pass_Depth;

void main(void)
{	
	// Send texture coordinates.
	pass_TexCoord = in_TexCoord;
	
	// Pass the position to the fragment shader in eye space.
	pass_Position = (modelMatrix * vec4(in_Position, 1.0)).xyz;
	
	// Pass the normal to the fragment shader in eye space.
	pass_Normal = (transpose(inverse(modelMatrix)) * vec4(in_Normal, 0.0)).xyz;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
	
	pass_Depth.x = gl_Position.z;
	pass_Depth.y = gl_Position.w;
}